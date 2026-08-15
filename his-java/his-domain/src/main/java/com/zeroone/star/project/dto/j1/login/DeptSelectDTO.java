package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import lombok.Data;

@Data
@Schema(description = "用户绑定选中科室入参")
public class DeptSelectDTO {

    @Schema(description = "选中科室id", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotEmpty(message = "选择一个科室")
    private String organizationUnitId;
}
