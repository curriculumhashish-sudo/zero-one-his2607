package com.zeroone.star.pharmacy.entity;

import lombok.Builder;
import lombok.Data;

import java.time.LocalDateTime;
import java.util.List;

//查询参数对象
@Builder
@Data
public class WarehouseInventoryDetailPageQuery {

    private List<Long> effectiveWarehouseIds;
    private List<Long> supplierIds;
    private List<String> medicationCategoryCodes;
    private List<String> deviceCategoryCodes;
    private List<Integer> chrgitmLvList;
    private List<String> inventoryStatusEnums;
    private Boolean zeroStockOnly;
    private Integer expireWithinDays;
    private LocalDateTime expireDeadline;
    private String keyword;
    private String keywordLower;
    private List<Long> resolvedMedicationDefinitionIds;
    private List<Long> resolvedDeviceDefinitionIds;
    private List<Long> resolvedMedicationDefinitionIdsForKeyword;
    private List<Long> resolvedDeviceDefinitionIdsForKeyword;
}