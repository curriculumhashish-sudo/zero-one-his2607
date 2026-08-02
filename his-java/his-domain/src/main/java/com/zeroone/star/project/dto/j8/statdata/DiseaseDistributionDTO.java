package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "患者疾病类型分布查询参数")
public class DiseaseDistributionDTO {

    @Schema(description = "年月（不传则默认当月）", example = "2026-07")
    private String yearMonth;

    @Schema(description = "返回 Top N", example = "10")
    private Integer topN = 10;

    @Schema(description = "科室ID（可选）", example = "dept001")
    private String deptId;
}
