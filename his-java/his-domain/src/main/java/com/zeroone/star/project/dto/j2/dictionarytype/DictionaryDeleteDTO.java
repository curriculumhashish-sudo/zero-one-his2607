package com.zeroone.star.project.dto.j2.dictionarytype;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 删除或停用字典项请求参数。
 */
@Data
@Schema(description = "删除或停用字典项请求参数")
public class DictionaryDeleteDTO {

    @NotBlank(message = "字典项ID不能为空")
    @Size(max = 64, message = "字典项ID长度不能超过64个字符")
    @Schema(description = "字典项ID，对应ter_concept.id", example = "10001",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @NotBlank(message = "处理方式不能为空")
    @Pattern(regexp = "DELETE|DISABLE", message = "处理方式只能是DELETE或DISABLE")
    @Schema(description = "处理方式：DELETE-逻辑删除，DISABLE-停用", example = "DELETE",
            allowableValues = {"DELETE", "DISABLE"}, requiredMode = Schema.RequiredMode.REQUIRED)
    private String action;
}
