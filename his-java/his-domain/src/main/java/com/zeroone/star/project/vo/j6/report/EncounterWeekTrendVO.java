package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 周接诊量门诊趋势 — 对齐 J8 TrendChartVO 结构（points + unit）。
 */
@Data
@Schema(description = "周接诊量门诊趋势")
public class EncounterWeekTrendVO {

    @Schema(description = "数据点列表")
    private List<TrendPointVO> points;

    @Schema(description = "单位", example = "人次")
    private String unit;
}
