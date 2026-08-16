package com.zeroone.star.project.dto.j5.emr;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.Data;

/**
 * <p>
 * 描述：病历模板创建DTO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "病历模板创建DTO")
public class MedicalTemplateCreateDTO {

    @NotBlank(message = "模板名称不能为空")
    @Size(max = 100, message = "模板名称不能超过100个字符")
    @Schema(description = "模板名称", requiredMode = Schema.RequiredMode.REQUIRED, example = "门诊病历模板")
    private String name;

    @Schema(description = "模板编码", example = "OUTPATIENT_TEMPLATE_001")
    private String code;

    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String documentTypeCode;

    @Schema(description = "文书类型ID", example = "1816800000000000001")
    private String documentTypeId;

    @Schema(description = "文书类型名称", example = "门诊病历")
    private String documentTypeName;

    @Schema(description = "机构ID", example = "1816800000000000001")
    private String organizationId;

    @Schema(description = "就诊分类枚举", example = "1")
    private Integer encounterClassEnum;

    @Schema(description = "就诊类型枚举", example = "1")
    private Integer encounterTypeEnum;

    @Schema(description = "模板内容(JSON)", example = "{\"fields\":[]}")
    private String content;

    @Schema(description = "是否启用", example = "true")
    private Boolean enabled;

    @Schema(description = "范围级别", example = "ORGANIZATION")
    private String scopeLevel;

    @Schema(description = "主诊医生ID", example = "1816800000000000001")
    private String ownerPractitionerId;

    @Schema(description = "就诊分类编码", example = "OUTPATIENT")
    private String encounterClassCode;

    @Schema(description = "模板分类", example = "OUTPATIENT")
    private String category;

    @Schema(description = "模板内容(JSON) - 兼容字段", example = "{\"fields\":[]}")
    private String contentJson;

    @Schema(description = "备注")
    private String remark;
}
