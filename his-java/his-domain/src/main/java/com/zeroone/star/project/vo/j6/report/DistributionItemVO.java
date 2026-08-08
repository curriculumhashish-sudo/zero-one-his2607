package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * 分布图数据项 — 对齐 J8 DistributionItemVO（name + value）。
 */
@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "分布图数据项")
public class DistributionItemVO {

    @Schema(description = "名称", example = "高血压")
    private String name;

    @Schema(description = "数值", example = "35")
    private Integer value;
}
