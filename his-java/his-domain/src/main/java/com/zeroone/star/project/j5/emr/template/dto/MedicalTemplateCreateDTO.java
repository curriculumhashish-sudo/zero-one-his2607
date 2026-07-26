package com.zeroone.star.project.j5.emr.template.dto;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonAlias;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.io.Serializable;

/**
 * 新增病历模板请求数据。
 */
@Data
@Schema(description = "新增病历模板请求")
public class MedicalTemplateCreateDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @NotBlank(message = "模板名称不能为空")
    @Size(max = 128, message = "模板名称不能超过128个字符")
    @JsonAlias("templateName")
    @Schema(description = "模板名称", example = "门诊病历（标准）", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Size(max = 64, message = "模板编码不能超过64个字符")
    @JsonAlias("templateCode")
    @Schema(description = "模板编码，非空时全库唯一", example = "OP_RECORD_STD")
    private String code;

    @NotBlank(message = "文书类型编码不能为空")
    @Size(max = 32, message = "文书类型编码不能超过32个字符")
    @JsonAlias("medicalType")
    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD", requiredMode = Schema.RequiredMode.REQUIRED)
    private String documentTypeCode;

    @Size(max = 64, message = "适用科室ID不能超过64个字符")
    @Schema(description = "适用科室ID，科室模板必填", example = "310400000000000123")
    private String organizationId;

    @Schema(description = "适用就诊类别旧枚举值")
    private Integer encounterClassEnum;

    @Schema(description = "适用就诊子类型")
    private Integer encounterTypeEnum;

    @NotBlank(message = "模板内容不能为空")
    @JsonAlias("templateContent")
    @Schema(description = "Tiptap/ProseMirror正文JSON", requiredMode = Schema.RequiredMode.REQUIRED)
    private String content;

    @JsonAlias("status")
    @Schema(description = "是否启用，默认true", example = "true")
    private Boolean enabled;

    @Size(max = 16, message = "使用范围不能超过16个字符")
    @Pattern(regexp = "(?i)\\s*(PERSONAL|DEPARTMENT|HOSPITAL)\\s*",
            message = "使用范围必须是 PERSONAL、DEPARTMENT 或 HOSPITAL")
    @Schema(description = "使用范围：PERSONAL/DEPARTMENT/HOSPITAL", example = "HOSPITAL")
    private String scopeLevel;

    @Size(max = 64, message = "个人模板所有者ID不能超过64个字符")
    @Schema(description = "个人模板所有者ID，个人模板必填")
    private String ownerPractitionerId;

    @NotBlank(message = "适用就诊类别不能为空")
    @Size(max = 16, message = "适用就诊类别不能超过16个字符")
    @Schema(description = "适用就诊类别：AMB/IMP/OBSENC等", example = "AMB", requiredMode = Schema.RequiredMode.REQUIRED)
    private String encounterClassCode;

    @AssertTrue(message = "科室模板必须选择适用科室")
    @JsonIgnore
    @Schema(hidden = true)
    public boolean isDepartmentScopeValid() {
        if (scopeLevel == null || !"DEPARTMENT".equalsIgnoreCase(scopeLevel.trim())) {
            return true;
        }
        return organizationId != null && !organizationId.trim().isEmpty();
    }
}
