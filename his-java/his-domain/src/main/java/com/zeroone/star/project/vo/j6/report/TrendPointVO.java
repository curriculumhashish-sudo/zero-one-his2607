package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 趋势图数据点 — 对齐 J8 TrendPointVO（label + value）。
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "趋势图数据点")
public class TrendPointVO {

    @Schema(description = "横轴标签", example = "2026-07-01")
    private String label;

    @Schema(description = "数值", example = "128")
    private Integer value;
}
