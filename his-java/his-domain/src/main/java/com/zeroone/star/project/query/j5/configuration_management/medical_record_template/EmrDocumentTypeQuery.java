package com.zeroone.star.project.query.j5.configuration_management.medical_record_template;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：文书类型查询
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "文书类型查询")
public class EmrDocumentTypeQuery extends PageQuery {

    @Schema(description = "关键词(编码或名称)", example = "门诊")
    private String keyword;

    @Schema(description = "文书类型编码", example = "OUTPATIENT_RECORD")
    private String code;

    @Schema(description = "文书类型名称", example = "门诊病历")
    private String name;

    @Schema(description = "是否启用", example = "1")
    private Integer isEnabled;
}
