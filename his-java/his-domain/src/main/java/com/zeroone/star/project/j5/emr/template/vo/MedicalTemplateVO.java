package com.zeroone.star.project.j5.emr.template.vo;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 病历模板视图对象
 */
@Getter
@Setter
@ToString
public class MedicalTemplateVO {

    @Schema(description = "模板ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    @Schema(description = "模板名称", example = "门诊病历模板", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateName;

    @Schema(description = "病历类型", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String medicalType;

    @Schema(description = "模板内容", example = "患者基本信息：姓名、年龄、性别...", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateContent;

    @Schema(description = "状态(0-禁用,1-启用)", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;

    @Schema(description = "创建时间", example = "2023-01-01 10:00:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private String createTime;

    @Schema(description = "更新时间", example = "2023-01-02 15:30:00", requiredMode = Schema.RequiredMode.REQUIRED)
    private String updateTime;

    @Schema(description = "创建人", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String createBy;

    @Schema(description = "更新人", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String updateBy;
}
