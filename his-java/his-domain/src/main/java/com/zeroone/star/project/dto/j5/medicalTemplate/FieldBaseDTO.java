package com.zeroone.star.project.dto.j5.medicalTemplate;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 写addDTO 与 update 的公共字段
 * 作者：青梅不甜
 */
@Data
@Schema(description = "写addDTO 与 update 的公共字段")
public class FieldBaseDTO {
    @Schema(description = "id/雪花算法自动生成",hidden = true, requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String id;
    @Schema(description = "fieldKey" , example = "patientName", requiredMode = Schema.RequiredMode.REQUIRED)
    private String fieldKey;

    @Schema(description = "显示名" , example = "患者姓名", requiredMode = Schema.RequiredMode.REQUIRED)
    private String label;

    @Schema(description = "数据类型" , example = "文本", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String dataType;

    @Schema(description = "取值来源" , example = "患者主档", requiredMode = Schema.RequiredMode.REQUIRED)
    private String valueSource;

    @Schema(description = "source_path" , example = "patient.name", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String sourcePath;

    @Schema(description = "默认值" , example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String defaultValue;

    @Schema(description = "默认可编辑" , example = "true", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer editableDefault;

    @Schema(description = "脱敏规则" , example = "无", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String maskRule;

    @Schema(description = "格式" , example = "日期", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String formatPattern;

    @Schema(description = "排序" , example = "0", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer sortOrder;

    @Schema(description = "启用" , example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Integer isEnabled;

    @Schema(description = "备注" , example = "无", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String remark;
}
