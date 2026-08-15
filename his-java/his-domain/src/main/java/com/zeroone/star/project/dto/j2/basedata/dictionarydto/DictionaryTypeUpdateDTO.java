package com.zeroone.star.project.dto.j2.basedata.dictionarydto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "修改编码类型请求参数")
public class DictionaryTypeUpdateDTO extends DictionaryTypeCreateDTO {

    @NotBlank(message = "编码类型ID不能为空")
    @Size(max = 64, message = "编码类型ID长度不能超过64个字符")
    @Schema(
            description = "编码类型ID（要修改的记录ID）",
            example = "208375890034369741",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String id;
}
