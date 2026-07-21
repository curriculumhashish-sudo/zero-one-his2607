package com.zeroone.star.project.dto.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.List;

@Getter
@Setter
@ToString
@Schema(description = "删除药品DTO")
public class MedicineDeleteDTO {
    @NotEmpty(message = "id不能为空")
    @Schema(description = "要删除的药品id列表",example = "[\"1\"]",defaultValue = "[\"1\"]")
    private List<String> ids;
}
