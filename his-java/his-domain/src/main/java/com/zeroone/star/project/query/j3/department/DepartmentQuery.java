package com.zeroone.star.project.query.j3.department;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：科室列表查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 很帅大王
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "科室列表查询条件")
public class DepartmentQuery extends PageQuery {

    @Schema(description = "关键词", example = "内科", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String keyword;

    @Schema(description = "状态", example = "ACTIVE", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String statusEnum;

    @Schema(description = "类型", example = "DEPARTMENT", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String typeEnum;

    @Schema(description = "类别", example = "CLINIC", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String classEnum;
}
