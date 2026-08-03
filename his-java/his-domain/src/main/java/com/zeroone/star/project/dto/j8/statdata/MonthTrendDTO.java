package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "月份流量趋势DTO")
public class MonthTrendDTO {

    @Schema(description = "日期(yyyy-MM-dd)", example = "2026-07-01")
    private String date;

    @Schema(description = "访问量", example = "3520")
    private Long visitCount;
}