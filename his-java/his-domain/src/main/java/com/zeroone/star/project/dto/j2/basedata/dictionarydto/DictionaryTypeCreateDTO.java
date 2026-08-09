package com.zeroone.star.project.dto.j2.basedata.dictionarydto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;
@Data
@Schema(description = "新增编码类型请求参数")
public class DictionaryTypeCreateDTO {

    @NotBlank(message = "编码类型编码不能为空")
    @Size(max = 100, message = "编码类型编码长度不能超过100个字符")
    @Schema(
            description = "编码类型编码（英文/拼音，唯一标识）",
            example = "gram",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String name;

    @NotBlank(message = "编码类型名称不能为空")
    @Size(max = 200, message = "编码类型名称长度不能超过200个字符")
    @Schema(
            description = "编码类型名称（中文显示名）",
            example = "克",
            requiredMode = Schema.RequiredMode.REQUIRED
    )
    private String title;

    @Size(max = 2000, message = "编码类型描述长度不能超过2000个字符")
    @Schema(
            description = "编码类型描述（可选）",
            example = "质量单位：克"
    )
    private String description;
}
