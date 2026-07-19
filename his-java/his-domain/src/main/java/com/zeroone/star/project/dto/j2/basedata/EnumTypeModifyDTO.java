package com.zeroone.star.project.dto.j2.basedata;


import io.swagger.v3.oas.annotations.media.Schema;
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
@ToString
@Schema(description = "枚举类型修改DTO，包含ID")
public class EnumTypeModifyDTO extends EnumTypeAddDTO {
    @Schema(description = "更新的枚举类型的ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
