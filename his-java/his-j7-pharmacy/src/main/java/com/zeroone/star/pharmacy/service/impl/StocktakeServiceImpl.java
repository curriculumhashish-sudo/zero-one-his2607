package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.StocktakeMapper;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeItemVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 描述：盘点单查询提交服务实现
 * </p>
 * <p>
 * 功能说明：基于 {@code wkf_supply_request} 表（{@code type_enum = INVENTORY_CHECK}）
 * 实现盘点单的列表分页查询、详情查询、提交审批三大业务功能。
 * 同一 {@code bus_no} 下聚合多条明细行，共享审批状态，提交审批时需批量更新。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Slf4j
@Service
public class StocktakeServiceImpl
        extends ServiceImpl<StocktakeMapper, WkfSupplyRequest>
        implements IStocktakeService {

    /**
     * 盘点单供应类型枚举
     */
    private static final String TYPE_ENUM_INVENTORY_CHECK = "INVENTORY_CHECK";

    /**
     * 盘点单状态枚举：草稿
     */
    private static final String STATUS_DRAFT = "DRAFT";

    /**
     * 盘点单状态枚举：待审批（已提交审批，与 SupplyStatus.PENDING_APPROVAL 一致）
     */
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";

    /**
     * 时间格式化器（用于 Mapper 参数传递）
     */
    private static final DateTimeFormatter DATE_TIME_FORMATTER =
            DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    @Resource
    private StocktakeMapper stocktakeMapper;

    @Resource
    private UserHolder userHolder;

    /**
     * 分页查询盘点单列表
     * <p>
     * 调用 Mapper 按 bus_no 聚合查询，将 LocalDateTime 类型的查询条件
     * 格式化为字符串以适配 SQL 比较。
     * </p>
     *
     * @param query 查询条件
     * @return 分页数据
     */
    @Override
    public PageDTO<StocktakeListVO> queryStocktakeList(StocktakeQuery query) {
        // 构造分页对象（PageQuery 中 pageIndex/pageSize 为 long）
        Page<StocktakeListVO> page = new Page<>(query.getPageIndex(), query.getPageSize());

        // LocalDateTime → String，避免 MyBatis 类型处理器对 LocalDateTime.toString() 输出 ISO 格式导致比较失败
        String startTimeStr = query.getStartTime() == null ? null
                : query.getStartTime().format(DATE_TIME_FORMATTER);
        String endTimeStr = query.getEndTime() == null ? null
                : query.getEndTime().format(DATE_TIME_FORMATTER);

        IPage<StocktakeListVO> result = stocktakeMapper.selectStocktakePage(
                page,
                query.getBusNo(),
                query.getStatusEnum(),
                query.getWarehouseId(),
                query.getApplicantId(),
                startTimeStr,
                endTimeStr
        );

        PageDTO<StocktakeListVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPages(result.getPages());
        pageDTO.setRows(result.getRecords());
        return pageDTO;
    }

    /**
     * 查询盘点单详情（含明细列表）
     * <p>
     * 一次查询聚合信息，一次查询明细列表，组装为详情 VO。
     * 若聚合信息不存在，直接返回 null。
     * </p>
     *
     * @param id 盘点单号（bus_no）
     * @return 盘点单详情；不存在返回 null
     */
    @Override
    public StocktakeDetailVO queryStocktakeDetail(String id) {
        // 1. 查询聚合信息
        StocktakeDetailVO detailVO = stocktakeMapper.selectStocktakeDetail(id);
        if (detailVO == null) {
            return null;
        }
        // 2. 查询明细列表
        List<StocktakeItemVO> items = stocktakeMapper.selectStocktakeItems(id);
        detailVO.setItems(items == null ? Collections.emptyList() : items);
        return detailVO;
    }

    /**
     * 提交盘点单审批
     * <p>
     * 业务校验：
     * 1. 单据必须存在且类型为 INVENTORY_CHECK
     * 2. 状态必须为 DRAFT
     * 3. 至少有一条明细行
     * </p>
     * <p>
     * 状态变更：DRAFT → PENDING_APPROVAL，同时写入申请人、申请时间、备注
     * </p>
     *
     * @param dto 提交审批参数
     * @return 提交结果消息
     * @throws IllegalArgumentException 业务校验失败
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public String submitStocktake(StocktakeSubmitDTO dto) {
        String busNo = dto.getId();
        // 1. 校验单据存在 & 类型为盘点单
        WkfSupplyRequest record = stocktakeMapper.selectOneByBusNo(busNo);
        if (record == null) {
            throw new IllegalArgumentException("盘点单不存在：" + busNo);
        }
        if (!TYPE_ENUM_INVENTORY_CHECK.equals(record.getTypeEnum())) {
            throw new IllegalArgumentException("单据类型非盘点单：" + busNo);
        }
        // 2. 校验状态为草稿
        if (!STATUS_DRAFT.equals(record.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "该盘点单不在草稿状态，无法提交审批，当前状态：" + record.getStatusEnum());
        }
        // 3. 校验至少有一条明细
        Long itemCount = stocktakeMapper.countItemsByBusNo(busNo);
        if (itemCount == null || itemCount <= 0) {
            throw new IllegalArgumentException("盘点单无明细行，无法提交审批：" + busNo);
        }

        // 4. 获取当前操作人作为申请人
        String applicantId = record.getApplicantId();
        String updatedBy = null;
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null) {
                updatedBy = currentUser.getUsername();
                // 若当前用户有 id，则以当前用户作为申请人
                if (currentUser.getId() != null && !currentUser.getId().isEmpty()) {
                    applicantId = currentUser.getId();
                }
            }
        } catch (Exception e) {
            log.warn("获取当前用户失败，使用单据原申请人：{}", e.getMessage());
        }

        // 5. 批量更新同一 bus_no 下所有行
        LocalDateTime now = LocalDateTime.now();
        int rows = stocktakeMapper.updateStatusByBusNo(
                busNo,
                STATUS_PENDING_APPROVAL,
                applicantId,
                now.format(DATE_TIME_FORMATTER),
                updatedBy,
                now.format(DATE_TIME_FORMATTER)
        );
        if (rows == 0) {
            throw new IllegalArgumentException("提交审批失败，单据可能已被其他操作修改：" + busNo);
        }
        log.info("盘点单 {} 提交审批成功，受影响行数：{}", busNo, rows);
        return "提交成功";
    }
}
