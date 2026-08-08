package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 流量趋势 — 对齐 J8 HourTrendVO 结构（points + unit）。
 */
@Data
@Schema(description = "流量趋势")
public class TrafficTrendVO {

    @Schema(description = "数据点列表")
    private List<TrendPointVO> points;

    @Schema(description = "单位", example = "次")
    private String unit;
}
