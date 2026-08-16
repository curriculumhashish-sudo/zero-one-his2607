package com.zeroone.star.project.dto.j5.Metadata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "元数据字段详情")
public class MetadataDetailDTO {

    @Schema(description = "唯一标识")
    private String id;

    @Schema(description = "字段标识", example = "patientName")
    private String fieldKey;

    @Schema(description = "显示名", example = "患者姓名")
    private String displayName;

    @Schema(description = "取值来源", example = "1")
    private String valueSource;

    @Schema(description = "编辑类型", example = "可改")
    private String editType;

    @Schema(description = "字段类型", example = "String")
    private String fieldType;

    @Schema(description = "长度", example = "128")
    private Integer length;

    @Schema(description = "精度", example = "0")
    private Integer precision;

    @Schema(description = "是否可空", example = "true")
    private Boolean nullable;

    @Schema(description = "默认值", example = "")
    private String defaultValue;

    @Schema(description = "描述", example = "患者姓名")
    private String description;

    @Schema(description = "排序", example = "1")
    private Integer sortOrder;

    @Schema(description = "状态", example = "1")
    private Integer status;
}