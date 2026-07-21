package com.zeroone.star.project.vo.documenttype;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 文书类型响应结果
 */
@Data
@Schema(description = "文书类型响应结果")
public class DocumentTypeVO {
    
    @Schema(description = "主键ID", example = "323100000000000101")
    private String id;
    
    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String code;
    
    @Schema(description = "文书类型名称", example = "门诊病历")
    private String name;
    
    @Schema(description = "默认模板ID", example = "323100000000000201")
    private String defaultTemplateId;
    
    @Schema(description = "是否启用", example = "true")
    private Boolean isEnabled;
    
    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;
    
    @Schema(description = "创建时间", example = "2026-07-19T17:00:00")
    private LocalDateTime createdAt;
    
    @Schema(description = "更新时间", example = "2026-07-19T17:00:00")
    private LocalDateTime updatedAt;
}
