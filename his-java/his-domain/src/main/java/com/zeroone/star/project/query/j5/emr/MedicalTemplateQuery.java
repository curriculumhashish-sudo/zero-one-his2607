package com.zeroone.star.project.query.j5.emr;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：病历模板查询
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "病历模板查询")
public class MedicalTemplateQuery extends PageQuery {

    @Schema(description = "模板名称", example = "门诊病历模板")
    private String templateName;

    @Schema(description = "模板名称(别名)", example = "门诊病历模板")
    private String name;

    @Schema(description = "文书类型ID", example = "1816800000000000001")
    private String documentTypeId;

    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String documentTypeCode;

    @Schema(description = "模板分类", example = "OUTPATIENT")
    private String category;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;
}
