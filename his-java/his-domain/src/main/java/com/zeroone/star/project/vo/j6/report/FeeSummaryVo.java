package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;

@Schema(description = "费用合计信息")
@Data
public class FeeSummaryVo {
    @Schema(description = "基准金额合计", example = "100.00")
    private BigDecimal baseAmountTotal;

    @Schema(description = "折扣金额合计", example = "100.00")
    private BigDecimal discountAmountTotal;

    @Schema(description = "应收金额合计", example = "100.00")
    private BigDecimal receivableAmountTotal;

    @Schema(description = "折后金额合计", example = "100.00")
    private BigDecimal discountedAmountTotal;

    @Schema(description = "实收金额合计", example = "100.00")
    private BigDecimal actualReceivedAmountTotal;

    @Schema(description = "医保金额合计", example = "100.00")
    private BigDecimal insuranceAmountTotal;

    @Schema(description = "自付金额合计", example = "100.00")
    private BigDecimal selfPayAmountTotal;

    @Schema(description = "行金额合计（数量×单价全量SUM）", example = "100.00")
    private BigDecimal lineAmountTotal;
}
