package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Schema(description = "启/停用诊断请求DTO")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class ToggleStatusDTO {

    @Schema(description = "目标状态", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "目标状态不能为空")
    private String valiFlag;
}