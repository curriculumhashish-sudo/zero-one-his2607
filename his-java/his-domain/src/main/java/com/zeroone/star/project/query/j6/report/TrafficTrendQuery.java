package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Pattern;
import lombok.Data;

@Schema(description = "流量趋势查询条件")
@Data
public class TrafficTrendQuery {

    @Schema(description = "开始日期", example = "2026-01-01", requiredMode = Schema.RequiredMode.REQUIRED)
    @Pattern(regexp = "\\d{4}-\\d{2}-\\d{2}", message = "日期格式不正确")
    private String start;

    @Schema(description = "结束日期", example = "2026-12-31", requiredMode = Schema.RequiredMode.REQUIRED)
    @Pattern(regexp = "\\d{4}-\\d{2}-\\d{2}", message = "日期格式不正确")
    private String end;

    @Schema(description = "统计粒度 (DAY:日, WEEK:周, MONTH:月)", example = "DAY")
    private String granularity;
}
