package com.zeroone.star.project.vo.j6.report;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.math.BigDecimal;

@Schema(description = "门诊费用列表数据")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class OutpatientFeeDetailsVo extends PageDTO<FeeDetailVO> {

    @Schema(description = "列表门诊费用汇总")
    OutpatientFeeSummaryVo summary;

    @Data
    @Schema(description = "门诊费用汇总信息")
    static class OutpatientFeeSummaryVo {

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
    }
}


