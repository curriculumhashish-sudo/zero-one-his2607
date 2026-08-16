package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * 患者疾病类型分布 — 对齐 J8 DistributionVO 结构（items + unit）。
 */
@Data
@Schema(description = "患者疾病类型分布")
public class DiseaseDistributionVO {

    @Schema(description = "数据项列表")
    private List<DistributionItemVO> items;

    @Schema(description = "单位", example = "人")
    private String unit;
}
