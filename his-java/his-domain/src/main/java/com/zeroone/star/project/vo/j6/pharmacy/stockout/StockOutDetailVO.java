package com.zeroone.star.project.vo.j6.pharmacy.stockout;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.util.List;

@Data
@Schema(description = "出库单详情")
public class StockOutDetailVO {

    @Schema(description = "出库单号", example = "OUT-20240719-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String busNo;

    @Schema(description = "类型枚举", example = "NORMAL", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String typeEnum;

    @Schema(description = "状态枚举", example = "APPROVED", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnum;

    @Schema(description = "源库位ID", example = "LOC-001", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String sourceLocationId;

    @Schema(description = "目的库位ID", example = "LOC-002", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String purposeLocationId;

    @Schema(description = "源类型枚举", example = "WAREHOUSE", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String sourceTypeEnum;

    @Schema(description = "目的类型枚举", example = "DEPARTMENT", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String purposeTypeEnum;

    @Schema(description = "分类枚举", example = "DRUG", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String categoryEnum;

    @Schema(description = "原始出库单号", example = "ORI-20240718-005", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String originalBusNo;

    @Schema(description = "出库单明细行", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private List<StockOutLineVO> lines;
}
