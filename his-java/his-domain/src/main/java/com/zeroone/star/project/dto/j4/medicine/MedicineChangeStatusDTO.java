package com.zeroone.star.project.dto.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.List;

@Setter
@Getter
@ToString
@Schema(description = "启/停用药品DTO(批量)")
public class MedicineChangeStatusDTO {
    @NotEmpty(message = "状态值不能为空")
    @Schema(example = "[\"1\"]",defaultValue = "[\"1\"]")
    private List<String> ids;
}
