package com.zeroone.star.project.dto.j2.basedata.dictionarytypedto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.PositiveOrZero;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 新增字典项请求参数。
 */
@Data
@Schema(description = "新增字典项请求参数")
public class DictionaryAddDTO {

    @NotBlank(message = "字典类型ID不能为空")
    @Size(max = 64, message = "字典类型ID长度不能超过64个字符")
    @Schema(description = "所属字典类型ID，对应ter_code_system.id", example = "900001",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String codeSystemId;

    @NotBlank(message = "字典编码不能为空")
    @Size(max = 100, message = "字典编码长度不能超过100个字符")
    @Schema(description = "字典编码，同一字典类型下唯一", example = "mg",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    @NotBlank(message = "字典名称不能为空")
    @Size(max = 500, message = "字典名称长度不能超过500个字符")
    @Schema(description = "字典名称", example = "毫克", requiredMode = Schema.RequiredMode.REQUIRED)
    private String display;

    @NotBlank(message = "字典描述不能为空")
    @Schema(description = "字典描述", example = "质量单位：毫克", requiredMode = Schema.RequiredMode.REQUIRED)
    private String definition;

    @PositiveOrZero(message = "排序号必须为非负整数")
    @Schema(description = "排序号，必须为非负整数，数值越小越靠前；不传时由服务端排在末尾", example = "10",
            requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Long sortOrder;
}
