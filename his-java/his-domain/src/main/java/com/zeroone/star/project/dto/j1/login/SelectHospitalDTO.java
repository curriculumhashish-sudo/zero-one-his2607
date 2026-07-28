package com.zeroone.star.project.dto.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "选择医院入参")
public class SelectHospitalDTO {
    @Schema(description = "医院/机构ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "h001")
    @NotBlank(message = "医院ID不能为空")
    private String hospitalId;
}