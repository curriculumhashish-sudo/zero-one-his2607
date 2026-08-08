package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 流量趋势查询 — 对齐 J8 BaseStatQueryDTO。
 */
@Data
@Schema(description = "流量趋势查询")
public class TrafficTrendQuery {

    @Schema(description = "开始日期", example = "2026-01-01", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateFrom;

    @Schema(description = "结束日期", example = "2026-12-31", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateTo;

    @Schema(description = "统计粒度", example = "day")
    private String granularity;

    @Schema(description = "机构ID", example = "322663105141149696")
    private String organizationId;

    /** @deprecated 使用 getDateFrom() */
    public String getStart() { return dateFrom; }
    /** @deprecated 使用 getDateTo() */
    public String getEnd() { return dateTo; }
}
