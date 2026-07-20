package com.zeroone.star.project.dto.j4.equipment;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "修改器材对象")
public class EquipmentUpdateDTO extends EquipmentAddDTO {
    @Schema(description = "主键", example = "320010001000000003", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
