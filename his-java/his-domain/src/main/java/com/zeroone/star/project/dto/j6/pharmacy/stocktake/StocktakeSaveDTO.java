package com.zeroone.star.project.dto.j6.pharmacy.stocktake;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.util.List;

/**
 * 盘点单 新增/保存/保存并提交 DTO
 * <p>busNo 由服务端生成，前端无需传入</p>
 * <p>明细行仅需传用户输入的字段，其余由服务端从数据库补全</p>
 */
@Data
public class StocktakeSaveDTO {

    // ==================== 头字段 ====================

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

        @Schema(description = "库存项ID（指向 wkf_inventory_item.id，用于快照锁定账面数）", requiredMode = Schema.RequiredMode.REQUIRED, example = "inv_001")
        private String inventoryItemId;

        @Schema(description = "盈亏数量（正=盘盈，负=盘亏，零=平）", example = "5.00")
        private BigDecimal profitLossQuantity;

        @Schema(description = "实盘数量（≥0）", example = "100.00")
        private BigDecimal actualQuantity;

        @Schema(description = "计量单位编码", requiredMode = Schema.RequiredMode.REQUIRED, example = "PCS")
        private String unitCode;

        @Schema(description = "批号", example = "LOT20260701")
        private String lotNumber;

        @Schema(description = "备注", example = "盘点正常")
        private String remake;

        @Schema(description = "追溯码", example = "TRACE001")
        private String traceNo;
    }

}
