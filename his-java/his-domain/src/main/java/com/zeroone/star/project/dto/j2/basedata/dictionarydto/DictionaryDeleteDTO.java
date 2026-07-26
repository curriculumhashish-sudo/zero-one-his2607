package com.zeroone.star.project.dto.j2.basedata.dictionarydto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 删除字典项请求参数。
 */
@Data
@Schema(description = "删除字典项请求参数")
public class DictionaryDeleteDTO {

    @NotBlank(message = "字典项ID不能为空")
    @Size(max = 64, message = "字典项ID长度不能超过64个字符")
    @Schema(description = "字典项ID，对应ter_concept.id", example = "10001",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
