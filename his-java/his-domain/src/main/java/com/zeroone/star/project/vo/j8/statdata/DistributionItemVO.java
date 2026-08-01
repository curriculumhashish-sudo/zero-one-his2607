package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 分布图数据项
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "分布图数据项")
public class DistributionItemVO {

    @Schema(description = "名称", example = "上呼吸道感染")
    private String name;

    @Schema(description = "数值", example = "1280")
    private Integer value;
}
