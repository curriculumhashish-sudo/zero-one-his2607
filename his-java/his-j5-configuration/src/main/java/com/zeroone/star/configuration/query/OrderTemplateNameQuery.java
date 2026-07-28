package com.zeroone.star.configuration.query;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;

@AllArgsConstructor
@Data
public class OrderTemplateNameQuery {

    @Schema(description = "适用场景（OUTPATIENT/INPATIENT/GENERAL）")
    private String sceneEnum = "OUTPATIENT";

    @Schema(description = "组套种类（GENERAL/TCM/PRICING）")
    private String templateKindEnum = "GENERAL";

    @Schema(description = "当前医生ID")
    private String practitionerId;

    @Schema(description = "当前科室 ID")
    private String deptId;
}
