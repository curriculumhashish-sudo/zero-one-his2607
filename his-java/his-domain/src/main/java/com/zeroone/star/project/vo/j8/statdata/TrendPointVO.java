package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 趋势图数据点
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "趋势图数据点")
public class TrendPointVO {

    @Schema(description = "横轴标签", example = "周一")
    private String label;

    @Schema(description = "数值", example = "128")
    private Integer value;
}
