package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "小时流量趋势数据点")
public class HourTrendDTO {

    @Schema(description = "小时标签(0-23)", example = "8")
    private String label;

    @Schema(description = "访问量", example = "156")
    private Long value;
}