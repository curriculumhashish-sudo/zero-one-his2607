package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import java.math.BigDecimal;

@Schema(description = "库存物资有效期预警数据点")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class ExpirationWarningVO {
    @Schema(description = "物资名称", example = "生理盐水")
    private String name;

    @Schema(description = "批号", example = "LOT20260601")
    private String lotNumber;

    @Schema(description = "失效日期 (yyyy-MM-dd)", example = "2026-09-30")
    private String expirationDate;

    @Schema(description = "剩余有效期天数", example = "73")
    private Integer remainingDays;

    @Schema(description = "当前库存数量", example = "500.00")
    private BigDecimal quantity;

    @Schema(description = "所属仓库名称", example = "西药房")
    private String warehouseName;

    @Schema(description = "供应商名称", example = "国药控股股份有限公司")
    private String supplierName;
}

