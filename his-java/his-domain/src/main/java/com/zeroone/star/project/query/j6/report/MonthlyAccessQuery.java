package com.zeroone.star.project.query.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Schema(description = "月访问量统计查询条件")
@Data
public class MonthlyAccessQuery {
    @Schema(description = "查询年份", example = "2026", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer year;
}

