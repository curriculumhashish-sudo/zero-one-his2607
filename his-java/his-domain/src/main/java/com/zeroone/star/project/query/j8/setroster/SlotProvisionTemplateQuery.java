package com.zeroone.star.project.query.j8.setroster;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：放号时段模板分页查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author Gin
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "放号时段模板分页查询条件")
public class SlotProvisionTemplateQuery extends PageQuery {

    @Schema(
            description = "搜索关键词，匹配模板编码或模板名称",
            example = "门诊上午"
    )
    private String keyword;

    @Schema(
            description = "机构/科室ID，为空则查询所有",
            example = "dept-001"
    )
    private String organizationId;

    @Schema(
            description = "是否启用",
            example = "true"
    )
    private Boolean enabled;
}
