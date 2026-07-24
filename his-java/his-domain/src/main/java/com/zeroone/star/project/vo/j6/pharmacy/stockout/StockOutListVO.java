package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "出库单列表行")
public class StockOutListVO {

    @Schema(description = "出库单号", example = "OUT-20240719-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String busNo;

    @Schema(description = "原始出库单号", example = "ORI-20240718-005", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String originalBusNo;

    @Schema(description = "类型枚举", example = "NORMAL", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String typeEnum;

    @Schema(description = "状态枚举", example = "APPROVED", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnum;

    @Schema(description = "分类枚举", example = "DRUG", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String categoryEnum;

    @Schema(description = "源库位ID", example = "LOC-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String sourceLocationId;

    @Schema(description = "目的库位ID", example = "LOC-002", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String purposeLocationId;

    @Schema(description = "供应商ID", example = "SUP-1001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String supplierId;

    @Schema(description = "物品ID", example = "ITEM-5001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemId;

    @Schema(description = "物品表名", example = "drug_info", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String itemTable;

    @Schema(description = "物品数量", example = "100", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer itemQuantity;

    @Schema(description = "单位编码", example = "BOX", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String unitCode;

    @Schema(description = "批号", example = "B20240701", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String lotNumber;

    @Schema(description = "追溯号", example = "TR20240719001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String traceNo;

    @Schema(description = "单价", example = "12.50", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer price;

    @Schema(description = "总价", example = "1250.00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer totalPrice;

    @Schema(description = "申请时间", example = "2024-07-19 10:30:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String applyTime;

    @Schema(description = "审批时间", example = "2024-07-19 14:00:00", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String approvalTime;
}
