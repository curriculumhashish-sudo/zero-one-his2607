package com.zeroone.star.project.j5.emr.template.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.time.LocalDateTime;

/**
 * 病历模板列表视图对象
 */
@Getter
@Setter
@ToString
public class MedicalTemplateVO {

    @Schema(description = "模板ID", example = "1816800000000000001")
    private String id;

    @Schema(description = "模板名称", example = "门诊病历模板")
    private String templateName;

    @Schema(description = "关联文书类型编码", example = "OUTPATIENT")
    private String documentTypeCode;

    @Schema(description = "模板内容（JSON摘要）", example = "{\"sections\":[...]}")
    private String templateContent;

    @Schema(description = "是否启用", example = "true")
    private Boolean isEnabled;

    @Schema(description = "使用范围：PERSONAL/DEPARTMENT/HOSPITAL", example = "HOSPITAL")
    private String scopeLevel;

    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;

    @Schema(description = "创建人", example = "admin")
    private String createdBy;

    @Schema(description = "更新人", example = "admin")
    private String updatedBy;
}
