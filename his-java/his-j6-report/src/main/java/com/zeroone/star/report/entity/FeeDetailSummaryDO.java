package com.zeroone.star.report.entity;

import lombok.Data;
import java.math.BigDecimal;

/**
 * 费用汇总 DO
 * @author J6
 */
@Data
public class FeeDetailSummaryDO {
    private BigDecimal baseAmountTotal;
    private BigDecimal discountAmountTotal;
    private BigDecimal receivableAmountTotal;
    private BigDecimal discountedAmountTotal;
    private BigDecimal actualReceivedAmountTotal;
    private BigDecimal insuranceAmountTotal;
    private BigDecimal selfPayAmountTotal;
}
