package com.zeroone.star.project.dto.documenttype;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * 新增文书类型请求参数
 */
@Data
@Schema(description = "新增文书类型请求参数")
public class DocumentTypeAddDTO {
    
    @NotBlank(message = "文书类型编码不能为空")
    @Size(max = 32, message = "编码长度不能超过32位")
    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String code;
    
    @NotBlank(message = "文书类型名称不能为空")
    @Size(max = 64, message = "名称长度不能超过64位")
    @Schema(description = "文书类型名称", example = "门诊病历")
    private String name;
    
    @Schema(description = "默认模板ID", example = "323100000000000201")
    private String defaultTemplateId;
    
    @Schema(description = "是否启用", example = "true")
    private Boolean isEnabled = true;
    
    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;
}
