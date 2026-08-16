package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.time.LocalDate;

/**
 * 月接诊量门诊趋势查询 — 对齐 J8 BaseStatQueryDTO。
 */
@Data
@Schema(description = "月接诊量门诊趋势查询")
public class EncounterMonthTrendQuery {
    @Schema(description = "开始日期", example = "2026-07-01")
    private String dateFrom;

    @Schema(description = "结束日期", example = "2026-07-31")
    private String dateTo;

    @Schema(description = "统计粒度", example = "day")
    private String granularity;

    @Schema(description = "机构ID", example = "322663105141149696")
    private String organizationId;

    public LocalDate getStartDate() {
        return dateFrom != null ? LocalDate.parse(dateFrom) : null;
    }

    public LocalDate getEndDate() {
        return dateTo != null ? LocalDate.parse(dateTo) : null;
    }
}
