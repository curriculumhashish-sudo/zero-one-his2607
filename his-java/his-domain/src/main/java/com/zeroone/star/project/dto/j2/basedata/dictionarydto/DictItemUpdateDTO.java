package com.zeroone.star.project.dto.j2.basedata.dictionarydto;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/*
字典更新的数据传输对象
 */
@Data
@Schema(description = "字典数据修改的传输对象")
public class DictItemUpdateDTO {
    @Schema(description = "id标识，不暴露给用户")
    private Long id;

    @Schema(description = "字典的编码不能修改")
    private String code;

    @NotBlank(message = "字典名称不能为空")
    @Schema(description = "字典显示名称", example = "住院")
    private String dictName;

    @NotBlank(message = "字典的描述不能为空")
    @Schema(description = "字典显示名称", example = "人没了")
    private String description;

    @Schema(description = "在字典中的排序")
    private Integer sort;
}
