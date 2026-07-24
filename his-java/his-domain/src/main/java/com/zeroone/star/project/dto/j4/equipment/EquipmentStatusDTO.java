package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "切换器材状态请求")
public class EquipmentStatusDTO {

    @NotBlank(message = "主键不能为空")
    @Schema(description = "主键", example = "320010001000000003", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
