package com.zeroone.star.project.dto.j2.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * 字典项更新数据传输对象
 */
@Data
@Schema(description = "字典项修改传输对象")
public class DictItemUpdateDTO {

    @Schema(description = "id标识，用户不可见")
    private Long id;

    @Schema(description = "字典id,用户不可见")
    private Long codeSystemId;

    @Schema(description = "字典项编码（只读）")
    private String code;

    @NotBlank(message = "字典项名称不能为空")
    @Schema(description = "字典项显示名称", example = "住院")
    private String display;

    @NotBlank(message = "字典项描述不能为空")
    @Schema(description = "字典项描述", example = "住院状态")
    private String definition;

    @Schema(description = "排序号",example = "1")
    private Integer sortOrder;

}
