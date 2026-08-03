package com.zeroone.star.project.query.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "库存图表查询参数")
public class StockChartQuery {

    @Schema(description = "起始月份(yyyy-MM)，不传则自动获取上半年(1-6月)或下半年(7-12月)", example = "2026-01")
    private String startMonth;

    @Schema(description = "结束月份(yyyy-MM)，不传则自动获取上半年(1-6月)或下半年(7-12月)", example = "2026-06")
    private String endMonth;
}