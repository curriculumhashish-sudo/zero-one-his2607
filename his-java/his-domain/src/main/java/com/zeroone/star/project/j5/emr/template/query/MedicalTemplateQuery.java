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

    @Schema(description = "模板名称（模糊查询）", example = "门诊")
    private String templateName;

    @Schema(description = "关联文书类型编码", example = "OUTPATIENT")
    private String documentTypeCode;

    @Schema(description = "是否启用", example = "true")
    private Boolean isEnabled;
}
