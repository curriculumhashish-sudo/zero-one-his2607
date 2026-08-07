package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

/**
 * 分布图返回数据
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "分布图返回数据")
public class DistributionVO {

    @Schema(description = "数据项列表")
    private List<DistributionItemVO> items;

    @Schema(description = "单位", example = "人")
    private String unit;
}
