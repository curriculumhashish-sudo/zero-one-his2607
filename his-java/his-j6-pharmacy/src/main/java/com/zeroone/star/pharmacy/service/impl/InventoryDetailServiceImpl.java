package com.zeroone.star.pharmacy.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.alibaba.cloud.commons.lang.StringUtils;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WarehouseInventoryDetailPageQuery;
import com.zeroone.star.pharmacy.mapper.AdmDeviceDefinitionMapper;
import com.zeroone.star.pharmacy.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.service.InventoryDetailService;
import com.zeroone.star.project.query.j6.pharmacy.inventory.InventoryDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.InventoryDetailVO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.time.LocalDate;
import java.time.LocalTime;
import java.util.List;
import java.util.Map;

@Service
public class InventoryDetailServiceImpl implements InventoryDetailService {

    @Autowired
    MedMedicationDefinitionMapper medicationDefinitionMapper;
    @Autowired
    AdmDeviceDefinitionMapper deviceDefinitionMapper;
    @Autowired
    WkfInventoryItemMapper inventoryItemMapper;

    public JsonVO<IPage<InventoryDetailVO>> pageInventoryDetailCurrent(InventoryDetailQuery request) {
        //创建query对象
        WarehouseInventoryDetailPageQuery query = buildQuery(request);

        // 预解析分类/关键字 → 定义ID列表
        enrichResolvedDefinitionIds(query);
        enrichKeywordDefinitionIds(query);

        // 分页条件查询
        Page<InventoryDetailVO> page = new Page<>(
                request.getPageIndex(), request.getPageSize());
        IPage<InventoryDetailVO> result =
                inventoryItemMapper.selectCurrentDetailPage(page, query);

        // 组装对象
        List<InventoryDetailVO> rows = result.getRecords();
        enrichRows(rows);

        // 返回分页结果
        Page<InventoryDetailVO> pageResult  = new Page<>();
        pageResult.setTotal(result.getTotal());
        pageResult.setRecords(rows);
        pageResult.setCurrent(request.getPageIndex());
        pageResult.setSize(request.getPageSize());

        return JsonVO.success(pageResult);
    }


    /** 创建查询对象, 用于后续分页查询传参 */
    private WarehouseInventoryDetailPageQuery buildQuery(InventoryDetailQuery request){

        WarehouseInventoryDetailPageQuery query = WarehouseInventoryDetailPageQuery.builder().build();
        BeanUtil.copyProperties(request,query);
        // 近效期截止时间
        if (query.getExpireWithinDays() != null && query.getExpireWithinDays() >= 0) {
            query.setExpireDeadline(LocalDate.now()
                    .plusDays(query.getExpireWithinDays())
                    .atTime(LocalTime.MAX));
        }

        // 4. 近效期截止时间
        if (query.getExpireWithinDays() != null && query.getExpireWithinDays() >= 0) {
            query.setExpireDeadline(LocalDate.now()
                    .plusDays(query.getExpireWithinDays())
                    .atTime(LocalTime.MAX));
        }

        // 关键字小写化
        if (StringUtils.isNotBlank(query.getKeyword())) {
            query.setKeywordLower(query.getKeyword().trim().toLowerCase());
        }

        return query;
    }

    /** 通过分类编码获取对应ids */
    private void enrichResolvedDefinitionIds(WarehouseInventoryDetailPageQuery query) {
        // 按药品分类编码查询 med_medication_definition 得到 ID 列表
        if (query.getMedicationCategoryCodes() != null && !query.getMedicationCategoryCodes().isEmpty()) {
            List<Long> ids = medicationDefinitionMapper.selectIdsByCategoryCodes(
                    query.getMedicationCategoryCodes());
            query.setResolvedMedicationDefinitionIds(ids);
        }

        // 按耗材分类编码查询 adm_device_definition 得到 ID 列表
        if (query.getDeviceCategoryCodes() != null && !query.getDeviceCategoryCodes().isEmpty()) {
            List<Long> ids = deviceDefinitionMapper.selectIdsByCategoryCodes(
                    query.getDeviceCategoryCodes());
            query.setResolvedDeviceDefinitionIds(ids);
        }

    }

    private void enrichKeywordDefinitionIds(WarehouseInventoryDetailPageQuery query) {
        if (StringUtils.isBlank(query.getKeyword())) {
            return;
        }
        String kw = query.getKeyword().trim();
        // 按编码/商品名等模糊匹配药品定义
        query.setResolvedMedicationDefinitionIdsForKeyword(
                medicationDefinitionMapper.selectIdsByKeyword(kw));
        // 按编码/商品名等模糊匹配耗材定义
        query.setResolvedDeviceDefinitionIdsForKeyword(
                deviceDefinitionMapper.selectIdsByKeyword(kw));
    }

    /** 库存状态枚举 → 展示文字 */
    private static final Map<String, String> INVENTORY_STATUS_MAP = Map.of(
            "ACTIVE", "正常",
            "INACTIVE", "停供"
    );

    /** 医保等级编码 → 展示文字 */
    private static final Map<String, String> CHRGITM_LV_MAP = Map.of(
            "1", "甲类",
            "2", "乙类",
            "3", "丙类"
    );

    /** 填充vo对象用于返回给前端 */
    private void enrichRows(List<InventoryDetailVO> rows) {
        if (rows == null || rows.isEmpty()) {
            return;
        }
        for (InventoryDetailVO row : rows) {
            // 1. 库存状态文字
            row.setInventoryStatusLabel(
                    INVENTORY_STATUS_MAP.getOrDefault(row.getInventoryStatusEnum(), row.getInventoryStatusEnum()));
            // 2. 医保等级文字
            if (row.getChrgitmLv() != null) {
                row.setChrgitmLvLabel(
                        CHRGITM_LV_MAP.getOrDefault(row.getChrgitmLv(), row.getChrgitmLv()));
            }

            // 3. 分类展示（暂无字典表，直接取编码）
            String categoryCode = row.getMedicationCategoryCode() != null
                    ? row.getMedicationCategoryCode()
                    : row.getDeviceCategoryCode();
            row.setCategoryLabel(categoryCode);

            // 4. 剂型展示（暂无字典表，直接取编码）
            row.setDoseFormLabel(row.getDoseFormCode());

            // 5. 包装单位名称（暂无字典表，直接取编码）
            row.setPackageUnitName(row.getPackageUnitCode());

            // 6. 包装库存文本 & 拆零库存文本
            buildStockText(row);
        }
    }

    /**
     * 计算包装库存文本和拆零库存文本
     * 例：totalVolume=1000, packageUnitRatio=24, packageUnitName=盒, splitUnitName=粒
     *   → packageStockText = "41盒"
     *   → splitStockText   = "1000粒"
     */
    private void buildStockText(InventoryDetailVO row) {
        BigDecimal total = row.getTotalVolume();
        if (total == null) {
            return;
        }
        // 拆零库存 = 总数量 + 拆零单位
        String splitUnit = row.getSplitUnitName() != null ? row.getSplitUnitName() : "";
        row.setSplitStockText(stripTrailingZeros(total) + splitUnit);

        // 包装库存 = 总数量 / 换算比例 + 包装单位
        BigDecimal ratio = row.getPackageUnitRatio();
        String pkgUnit = row.getPackageUnitName() != null ? row.getPackageUnitName() : "";
        if (ratio != null && ratio.compareTo(BigDecimal.ZERO) > 0) {
            BigDecimal pkgQty = total.divide(ratio, 0, RoundingMode.FLOOR);
            row.setPackageStockText(stripTrailingZeros(pkgQty) + pkgUnit);
        } else {
            row.setPackageStockText(stripTrailingZeros(total) + pkgUnit);
        }
    }

    /** 去除 BigDecimal 尾部多余的 0，如 1000.00 → 1000 */
    private String stripTrailingZeros(BigDecimal value) {
        return value.stripTrailingZeros().toPlainString();
    }

}