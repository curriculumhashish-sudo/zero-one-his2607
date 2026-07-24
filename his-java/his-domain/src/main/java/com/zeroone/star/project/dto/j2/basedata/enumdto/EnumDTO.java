package com.zeroone.star.project.dto.j2.basedata.enumdto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述: 枚举值DTO，其不包含租户，创建时间等信息,为的是简化传输的信息
 * </p>
 * @version 2.0
 */
@Data
@Schema(description = "枚举类型分页查询对象")
public class EnumDTO extends EnumAddDTO {

    @NotBlank
    @Schema(description = "枚举ID", example = "exdat1234123047", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
