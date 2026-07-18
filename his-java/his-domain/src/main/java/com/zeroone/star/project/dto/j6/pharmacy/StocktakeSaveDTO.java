package com.zeroone.star.project.dto.j6.pharmacy;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

/**
 * 盘点单 新增/保存/保存并提交 DTO
 * <p>新增时 busNo 为空，仅需头字段（purposeLocationId、categoryEnum），服务端生成空明细行</p>
 * <p>保存/提交时 busNo 必填，需携带完整明细行列表</p>
 */
@Data
public class StocktakeSaveDTO {

    // ==================== 头字段 ====================

    @Schema(description = "单据号（新增时为空，保存/提交时必填）", example = "PD202607180001")
    private String busNo;

    @Schema(description = "库房ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "284503862625177600")
    private String purposeLocationId;

    @Schema(description = "盘点人ID", example = "322733572165734400")
    private String practitionerId;

    @Schema(description = "单据类型枚举", requiredMode = Schema.RequiredMode.REQUIRED, example = "WAREHOUSE_STOCKTAKE")
    private String categoryEnum;

    // ==================== 明细行 ====================

    @Schema(description = "明细行列表（新增时可为空；保存/提交时必填）")
    private List<StocktakeLineDTO> lines;

    // ==================== 内嵌类 ====================

    @Data
    public static class StocktakeLineDTO{
        @Schema(description = "物品定义表名", requiredMode = Schema.RequiredMode.REQUIRED, example = "medication")
        private String itemTable;

        @Schema(description = "物品定义ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "med_001")
        private String itemId;

        @Schema(description = "库存项ID（指向 wkf_inventory_item.id，用于快照锁定账面数）", requiredMode = Schema.RequiredMode.REQUIRED, example = "inv_001")
        private String inventoryItemId;

        @Schema(description = "盈亏数量（正=盘盈，负=盘亏，零=平）", example = "5.00")
        private BigDecimal profitLossQuantity;

        @Schema(description = "实盘数量（≥0）", example = "100.00")
        private BigDecimal actualQuantity;

        @Schema(description = "盈亏金额（正=盘盈，负=盘亏）", example = "125.50")
        private BigDecimal profitLossAmount;

        @Schema(description = "计量单位编码", requiredMode = Schema.RequiredMode.REQUIRED, example = "PCS")
        private String unitCode;

        @Schema(description = "批号", example = "LOT20260701")
        private String lotNumber;

        @Schema(description = "单价", example = "25.10")
        private BigDecimal price;

        @Schema(description = "发生时间（盘点执行时刻）", example = "2026-07-18 10:00:00")
        private LocalDateTime occurrenceTime;

        @Schema(description = "备注", example = "盘点正常")
        private String remake;

        @Schema(description = "供应商标识", example = "supplier_001")
        private String supplierId;

        @Schema(description = "追溯码", example = "TRACE001")
        private String traceNo;
    }

}
