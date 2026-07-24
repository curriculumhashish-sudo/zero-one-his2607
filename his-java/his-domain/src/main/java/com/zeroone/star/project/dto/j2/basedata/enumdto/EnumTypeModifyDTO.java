package com.zeroone.star.project.dto.j2.basedata.enumdto;


import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import lombok.ToString;

/**
 * <p>
 * 描述：枚举类型修改DTO
 * </p>
 * @author 逆光 ni_guang
 * @version 1.0
 */
@Data
@Schema(description = "枚举类型修改DTO，包含ID")
public class EnumTypeModifyDTO extends EnumTypeAddDTO {

    @NotBlank(message = "更新的枚举类型的ID不能为空")
    @Schema(description = "更新的枚举类型的ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
