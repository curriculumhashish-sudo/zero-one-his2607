package com.zeroone.star.project.dto.ConfigurationManagement;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "执行科室名称")
public class OrgNameDTO {

    @Schema(description = "执行科室名称", example = "门诊内科", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;
}
