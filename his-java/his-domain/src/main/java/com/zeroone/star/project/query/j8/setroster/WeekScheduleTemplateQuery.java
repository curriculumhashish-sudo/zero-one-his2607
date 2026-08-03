package com.zeroone.star.project.query.j8.setroster;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：周排班模板查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "周排班模板查询条件")
public class WeekScheduleTemplateQuery extends PageQuery {

    @Schema(description = "关键词（模糊搜索：编码/名称）", example = "内科周模板")
    private String keyword;

    @Schema(description = "机构/科室ID", example = "org-001")
    private String organizationId;

    @Schema(description = "启用状态", example = "true")
    private Boolean enabled;
}
