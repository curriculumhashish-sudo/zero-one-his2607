package com.zeroone.star.project.j5.emr.template.query;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * 病历模板查询条件
 */
@Getter
@Setter
@ToString
public class MedicalTemplateQuery extends PageQuery {

    @Schema(description = "模板名称", example = "门诊病历模板")
    private String templateName;

    @Schema(description = "病历类型", example = "1")
    private String medicalType;
}
