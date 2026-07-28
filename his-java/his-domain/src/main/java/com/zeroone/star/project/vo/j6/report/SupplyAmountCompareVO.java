package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.math.BigDecimal;

@Schema(description = "物资出入库金额对比数据点")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class SupplyAmountCompareVO {
    @Schema(description = "交易类型 (INBOUND:入库, OUTBOUND:出库)", example = "INBOUND")
    private String type;

    @Schema(description = "总金额(元)", example = "22500.00")
    private BigDecimal totalAmount;

    @Schema(description = "统计期间(格式：yyyy-MM)", example = "2026-06")
    private String period;
}
