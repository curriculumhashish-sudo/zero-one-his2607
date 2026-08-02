package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Schema(description = "疾病类型分布扇形图数据项")
@AllArgsConstructor
@NoArgsConstructor
public class DiseaseDistributionItemVO {

    @Schema(description = "疾病名称", example = "上呼吸道感染",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String diseaseName;

    @Schema(description = "患者数量", example = "1280",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer count;

    @Schema(description = "占比（百分比数值，如 34.5 表示 34.5%）",
            example = "34.5", requiredMode = Schema.RequiredMode.REQUIRED)
    private Double percentage;
}
