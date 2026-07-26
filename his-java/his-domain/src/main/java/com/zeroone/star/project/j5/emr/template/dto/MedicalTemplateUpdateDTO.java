package com.zeroone.star.project.j5.emr.template.dto;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonAlias;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.Data;

import java.io.Serializable;

/**
 * 修改病历模板请求数据，非空字段会覆盖原值。
 */
@Data
@Schema(description = "修改病历模板请求")
public class MedicalTemplateUpdateDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Size(max = 128, message = "模板名称不能超过128个字符")
    @JsonAlias("templateName")
    @Schema(description = "模板名称", example = "门诊病历（新版）")
    private String name;

    @Size(max = 64, message = "模板编码不能超过64个字符")
    @JsonAlias("templateCode")
    @Schema(description = "模板编码，非空时全库唯一", example = "OP_RECORD_V2")
    private String code;

    @Size(max = 32, message = "文书类型编码不能超过32个字符")
    @JsonAlias("medicalType")
    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String documentTypeCode;

    @Size(max = 64, message = "适用科室ID不能超过64个字符")
    @Schema(description = "适用科室ID")
    private String organizationId;

    @Schema(description = "适用就诊类别旧枚举值")
    private Integer encounterClassEnum;

    @Schema(description = "适用就诊子类型")
    private Integer encounterTypeEnum;

    @JsonAlias("templateContent")
    @Schema(description = "Tiptap/ProseMirror正文JSON")
    private String content;

    @JsonAlias("status")
    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;

    @Size(max = 16, message = "使用范围不能超过16个字符")
    @Pattern(regexp = "(?i)\\s*(PERSONAL|DEPARTMENT|HOSPITAL)\\s*",
            message = "使用范围必须是 PERSONAL、DEPARTMENT 或 HOSPITAL")
    @Schema(description = "使用范围：PERSONAL/DEPARTMENT/HOSPITAL")
    private String scopeLevel;

    @Size(max = 64, message = "个人模板所有者ID不能超过64个字符")
    @Schema(description = "个人模板所有者ID")
    private String ownerPractitionerId;

    @Size(max = 16, message = "适用就诊类别不能超过16个字符")
    @Schema(description = "适用就诊类别：AMB/IMP/OBSENC等")
    private String encounterClassCode;

    @AssertTrue(message = "至少提供一个需要修改的字段")
    @JsonIgnore
    @Schema(hidden = true)
    public boolean isUpdateRequested() {
        return name != null
                || code != null
                || documentTypeCode != null
                || organizationId != null
                || encounterClassEnum != null
                || encounterTypeEnum != null
                || content != null
                || enabled != null
                || scopeLevel != null
                || ownerPractitionerId != null
                || encounterClassCode != null;
    }
}
