package com.zeroone.star.project.vo.j5.emr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：病历模板VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@Schema(description = "病历模板VO")
public class MedicalTemplateVO {

    @Schema(description = "模板ID", example = "1816800000000000001")
    private String id;

    @Schema(description = "模板名称", example = "门诊病历模板")
    private String templateName;

    @Schema(description = "文书类型ID", example = "1816800000000000001")
    private String documentTypeId;

    @Schema(description = "文书类型名称", example = "门诊病历")
    private String documentTypeName;

    @Schema(description = "模板分类", example = "OUTPATIENT")
    private String category;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;

    @Schema(description = "创建时间")
    private String createTime;

    @Schema(description = "更新时间")
    private String updateTime;
}
