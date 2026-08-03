package com.zeroone.star.outpatient.service.impl;

import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.outpatient.mapper.MinXiMapper;
import com.zeroone.star.outpatient.mapper.MinXiStructMapper;
import com.zeroone.star.outpatient.service.MinXiService;
import com.zeroone.star.project.components.excel.ExcelComponent;
import com.zeroone.star.project.components.fastdfs.FastDfsClientComponent;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j8.mzfymx.MinXiDTO;
import com.zeroone.star.project.dto.j8.mzfymx.MinXiExportDTO;
import com.zeroone.star.project.query.j8.mzfymx.MinXiExportQuery;
import com.zeroone.star.project.query.j8.mzfymx.MinXiQuery;
import com.zeroone.star.project.vo.j8.mzfymx.ExportFileVO;
import com.zeroone.star.project.vo.j8.mzfymx.MinXiPageVO;
import com.zeroone.star.project.components.fastdfs.FastDfsFileInfo;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：门诊费用明细（mzfymx）Service 实现类（具体业务逻辑写在这里，接口在 {@link MinXiService}）
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Service
public class MinXiServiceImpl implements MinXiService {

    // ========== 核心数据访问层 ==========
    @Resource
    private MinXiMapper minXiMapper;

    // ========== 公共组件（导出功能用，注入后 export 方法实现直接用）==========
    @Resource
    private ExcelComponent excelComponent;

    @Resource
    private FastDfsClientComponent fastDfsClientComponent;

    @Resource
    private UserHolder userHolder;

    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    /**
     * FastDFS Nginx 下载地址前缀（从 Nacos 配置 fastdfs.nginx-servers 读取）
     * 如：192.168.30.132:8888 → 自动拼接 http:// 前缀
     */
    @Value("${fastdfs.nginx-servers:192.168.30.132:8888}")
    private String dfsNginxServers;

    /**
     * 日期格式化（yyyy-MM-dd，目前业务未用，保留供后续扩展）
     */
    @SuppressWarnings("unused")
    private static final DateTimeFormatter DATE_FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    /**
     * 导出文件名时间戳格式（yyyyMMddHHmmss）
     */
    private static final DateTimeFormatter FILE_TS_FMT = DateTimeFormatter.ofPattern("yyyyMMddHHmmss");

    /**
     * Excel MIME 类型（.xlsx）
     */
    private static final String XLSX_CONTENT_TYPE =
            "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";

    /**
     * 获取当前请求的租户ID
     * <p>数据来源：Redis Hash {@code his:login:context:{userId}} → field {@code tenantId}</p>
     * @return 租户ID
     */
    private String getCurrentTenantId() {
        try {
            String userId = userHolder.getCurrentUser().getId();
            Object tenantObj = redisTemplate.opsForHash().get("his:login:context:" + userId, "tenantId");
            if (tenantObj != null) {
                return tenantObj.toString();
            }
        } catch (Exception ignored) {
        }
        return "1";
    }


    /**
     * 接口 1：分页查询门诊费用明细（方案 B 纯平铺，1 行 = 1 条收费项，同一次就诊的就诊字段每行重复）
     *        返回 MinXiPageVO = rows + totalRowCount + pageNo/pageSize(新增返回) + 8 个 *Sum(全量合计)
     *  实现步骤：
     *  ① selectTotals(query)  → 一次性查出 8 个金额合计（全量 SUM，不带 LIMIT）
     *  ② selectRows(new Page(pageIndex, pageSize), query)
     *                       → MyBatis-Plus 分页，自动加 LIMIT / 查 total
     *  ③ set sequenceNo 序号 = (pageIndex-1)*pageSize + i+1（跨页连续递增）
     *  ④ set pageNo / pageSize = 入参原值（前端分页栏高亮用，不动 PageQuery 父类）
     */
    @Override
    @Transactional(readOnly = true)
    public MinXiPageVO queryPage(MinXiQuery query) {
        query.setTenantId(getCurrentTenantId());

        // ① 全量合计（不带 LIMIT，原型图顶部红色数字）
        MinXiPageVO totals = minXiMapper.selectTotals(query);
        if (totals == null) {
            totals = new MinXiPageVO();
        }

        // ② 分页明细
        //   ⚠️ pageIndex/pageSize 继承自 PageQuery（基本类型 long，默认值 0，不可能为 null）
        //      ⚠️ @Min(value=1) 校验：如果前端传 0/-1，<1 会被全局异常处理器拦截，Service 层兜底 <1 给默认值
        long pageIndex = query.getPageIndex() < 1 ? 1 : query.getPageIndex();
        long pageSize  = query.getPageSize()  < 1 ? 10 : query.getPageSize();
        Page<MinXiDTO> mpPage = new Page<>(pageIndex, pageSize);
        List<MinXiDTO> rows = minXiMapper.selectRows(mpPage, query);
        if (rows == null) {
            rows = new ArrayList<>();
        }

        // ③ set 分页序号（sequenceNo 是前端列表的第 1 列"序号"，跨页连续递增）
        long startNo = (pageIndex - 1) * pageSize + 1;
        for (int i = 0; i < rows.size(); i++) {
            rows.get(i).setSequenceNo(startNo + i);
        }

        // 组装返回
        totals.setContent(rows);
        totals.setTotalCount(mpPage.getTotal());
        // ④ 你明确要求新增返回 pageNo / pageSize（不动 PageQuery 父类，只在返回值里加这俩字段）
        totals.setPageNo(pageIndex);
        totals.setPageSize(pageSize);
        return totals;
    }

    /**
     * 接口 2：按查询条件生成 Excel 报表 → 上传 FastDFS → 返回下载 URL（完全对齐参考项目）
     *  ⚠️ 纯 URL 模式：body 恒为 FastDFS 下载地址（http://nginx-servers/group1/M00/xxx.xlsx?token=xxx）
     *                   前端 window.open(resp.data.body) 直接下载
     *                   FastDFS 上传/生成URL失败直接抛异常，不降级，完全对齐参考项目行为
     *
     * 实现步骤：
     *  ① selectAllRows(exportQuery)     → 全量明细（不带 LIMIT，复用 selectRows 的 SELECT/JOIN/WHERE）
     *  ② MapStruct toExportDTOList      → MinXiDTO → MinXiExportDTO（27列，tradeDirection 英文→中文）
     *  ③ selectTotals(minXiQuery)       → 全量 8 个金额合计（调用 selectTotals 需要 MinXiQuery，BeanUtils 拷贝）
     *  ④ buildTotalRow(totals)          → 构造合计行（第1列门诊号填"合计"，8个金额列填 SUM）
     *  ⑤ 追加合计行到 exportRows 末尾
     *  ⑥ ExcelComponent.export()        → 写 ByteArrayOutputStream（内部自动 5000 条分 Sheet）
     *  ⑦ fastDfsClientComponent.uploadFile(byte[], "xlsx") → 上传 FastDFS
     *  ⑧ fetchUrl(info, urlPrefix, isToken=true)            → 生成下载 URL（带防盗链 Token）
     *  ⑨ 组装 ExportFileVO（body = 完整 FastDFS 下载 URL）
     */
    @Override
    @Transactional(readOnly = true)
    public ExportFileVO export(MinXiExportQuery exportQuery) {
        exportQuery.setTenantId(getCurrentTenantId());

        // ① 全量明细（不带 LIMIT）
        List<MinXiDTO> allDTOs = minXiMapper.selectAllRows(exportQuery);
        if (allDTOs == null) {
            allDTOs = new ArrayList<>();
        }

        // ② MinXiDTO → MinXiExportDTO（BeanUtils 手动映射 + 收退方向翻译，100% 可控）
        List<MinXiExportDTO> exportRows = new ArrayList<>(allDTOs.size() + 1);
        for (MinXiDTO dto : allDTOs) {
            MinXiExportDTO row = new MinXiExportDTO();
            BeanUtils.copyProperties(dto, row);
            row.setTradeDirection(translateTradeDirection(dto.getTradeDirection()));
            exportRows.add(row);
        }

        // ③ 全量金额合计（selectTotals 入参是 MinXiQuery，9个筛选字段 BeanUtils 拷贝）
        MinXiQuery minXiQuery = convertToMinXiQuery(exportQuery);
        MinXiPageVO totals = minXiMapper.selectTotals(minXiQuery);
        if (totals == null) {
            totals = new MinXiPageVO();
        }

        // 构造合计行并追加到末尾
        MinXiExportDTO totalRow = buildTotalRow(totals);
        exportRows.add(totalRow);

        // 写 Excel 到内存流（自动 5000 条/Sheet 分页）
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            excelComponent.export("门诊费用明细", baos, MinXiExportDTO.class, exportRows);
        } catch (IOException e) {
            throw new RuntimeException("生成门诊费用明细Excel失败：" + e.getMessage(), e);
        }
        byte[] xlsxBytes = baos.toByteArray();

        // 上传 FastDFS（byte[] 直接上传，不写临时文件）
        FastDfsFileInfo dfsInfo;
        try {
            dfsInfo = fastDfsClientComponent.uploadFile(xlsxBytes, "xlsx");
        } catch (Exception e) {
            throw new RuntimeException("上传门诊费用明细Excel到FastDFS失败：" + e.getMessage(), e);
        }
        if (dfsInfo == null) {
            throw new RuntimeException("上传门诊费用明细Excel到FastDFS失败：返回文件信息为空");
        }

        // 生成下载 URL（拼接 Nginx 地址 + 防盗链 Token，isToken=true 完全对齐配置 http-anti-steal-token=true）
        String urlPrefix = buildUrlPrefix(dfsNginxServers);
        String downloadUrl = fastDfsClientComponent.fetchUrl(dfsInfo, urlPrefix, true);
        if (downloadUrl == null || downloadUrl.isEmpty()) {
            throw new RuntimeException("生成FastDFS下载地址失败");
        }

        // 组装返回（body = 完整下载 URL，前端 window.open(resp.data.body) 直接下载）
        String fileName = "门诊费用明细_" + LocalDateTime.now().format(FILE_TS_FMT) + ".xlsx";
        return new ExportFileVO(fileName, XLSX_CONTENT_TYPE, downloadUrl);
    }

    // 辅助方法：把 fastdfs.nginx-servers 转成标准 URL 前缀
    //   输入：192.168.30.132:8888  或  http://192.168.30.132:8888
    //         或多个逗号分隔 192.168.30.132:8888,192.168.30.133:8888
    //   输出：http://192.168.30.132:8888
    private String buildUrlPrefix(String nginxServers) {
        if (nginxServers == null || nginxServers.isEmpty()) {
            throw new RuntimeException("fastdfs.nginx-servers 配置为空");
        }
        // 取第一个（逗号分隔时）
        String first = nginxServers.split(",")[0].trim();
        // 补协议前缀
        if (!first.startsWith("http://") && !first.startsWith("https://")) {
            first = "http://" + first;
        }
        // 去掉尾部斜杠
        while (first.endsWith("/")) {
            first = first.substring(0, first.length() - 1);
        }
        return first;
    }

    // 辅助方法：MinXiExportQuery → MinXiQuery 转换（9个筛选字段 BeanUtils 拷贝）

    private MinXiQuery convertToMinXiQuery(MinXiExportQuery exportQuery) {
        MinXiQuery minXiQuery = new MinXiQuery();
        // 9个筛选字段完全同名同类型，BeanUtils 自动拷贝
        BeanUtils.copyProperties(exportQuery, minXiQuery);
        return minXiQuery;
    }

    // 辅助方法：构造合计行（门诊号列填"合计"，8个金额列填全量 SUM）
    private MinXiExportDTO buildTotalRow(MinXiPageVO totals) {
        MinXiExportDTO totalRow = new MinXiExportDTO();
        // 第 1 列填"合计"，醒目标识
        totalRow.setOutpatientNo("合计");
        // ======= 8 个金额列填全量 SUM（注意用 *Sum 字段）=======
        totalRow.setBaseAmount(totals.getBaseAmountSum());
        totalRow.setDiscountAmount(totals.getDiscountAmountSum());
        totalRow.setReceivableAmount(totals.getReceivableAmountSum());
        totalRow.setDiscountedAmount(totals.getDiscountedAmountSum());
        totalRow.setLineAmount(totals.getLineAmountSum());
        totalRow.setActualReceivedAmount(totals.getActualReceivedAmountSum());
        totalRow.setInsuranceAmount(totals.getInsuranceAmountSum());
        totalRow.setSelfPayAmount(totals.getSelfPayAmountSum());
        // 其他 18 个展示列保持 null（Excel 中显示为空，符合常规报表习惯）
        return totalRow;
    }

    // ======================================================================
    // 辅助方法：收退方向英文编码 → 中文展示
    //   CHARGE → 收费
    //   REFUND → 退费
    //   其他 → 原值返回（兜底）
    // ======================================================================
    private String translateTradeDirection(String code) {
        if ("CHARGE".equals(code)) {
            return "收费";
        } else if ("REFUND".equals(code)) {
            return "退费";
        }
        return code;
    }
}