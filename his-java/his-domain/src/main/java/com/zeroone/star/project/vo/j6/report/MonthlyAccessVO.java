package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 月访问量 — 对齐 J8 TrendChartVO 结构（points + unit）。
 */
@Data
@Schema(description = "月访问量趋势")
public class MonthlyAccessVO {

    @Schema(description = "数据点列表")
    private List<TrendPointVO> points;

    @Schema(description = "单位", example = "次")
    private String unit;
}
