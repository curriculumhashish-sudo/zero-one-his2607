package com.zeroone.star.project.dto.documenttype;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

/**
 * <p>
 * 描述：文书类型更新DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "文书类型更新DTO")
public class DocumentTypeUpdateDTO {

    @NotBlank(message = "文书类型ID不能为空")
    @Schema(description = "文书类型ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1816800000000000001")
    private String id;

    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String code;

    @Schema(description = "文书类型名称", example = "门诊病历")
    private String name;

    @Schema(description = "默认模板ID", example = "1816800000000000001")
    private String defaultTemplateId;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;

    @Schema(description = "排序号", example = "0")
    private Integer sortOrder;
}
