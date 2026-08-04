package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

/**
 * 趋势图返回数据
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "趋势图返回数据")
public class TrendChartVO {

    @Schema(description = "数据点列表")
    private List<TrendPointVO> points;

    @Schema(description = "单位", example = "人次")
    private String unit;
}
