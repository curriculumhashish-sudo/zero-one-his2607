package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Schema(description = "物资出入库金额对比查询条件")
@Data
public class SupplyAmountCompareQuery {
    @Schema(description = "查询开始日期", example = "2026-01-01", requiredMode = Schema.RequiredMode.REQUIRED)
    private String start;

    @Schema(description = "查询结束日期", example = "2026-06-30", requiredMode = Schema.RequiredMode.REQUIRED)
    private String end;
}
