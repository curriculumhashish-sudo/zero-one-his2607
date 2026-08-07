package com.zeroone.star.project.query.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "库存图表查询参数")
public class StockChartQuery {

    @Schema(description = "开始日期(yyyy-MM-dd)", example = "2026-07-20")
    private String dateFrom;

    @Schema(description = "结束日期(yyyy-MM-dd)", example = "2026-07-20")
    private String dateTo;

    @Schema(description = "粒度", example = "day")
    private String granularity;

    @Schema(description = "组织ID", example = "org001")
    private String organizationId;
}