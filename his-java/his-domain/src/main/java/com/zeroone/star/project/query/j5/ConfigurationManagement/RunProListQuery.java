package com.zeroone.star.project.query.j5.ConfigurationManagement;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * <p>
 * 描述：执行项目列表查询
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "执行项目列表查询")
public class RunProListQuery extends PageQuery {

    @Schema(description = "项目名称", example = "血常规")
    private String itemName;

    @Schema(description = "项目编码", example = "CBC001")
    private String itemCode;

    @Schema(description = "执行科室ID", example = "1816800000000000001")
    private String execDeptId;

    @Schema(description = "是否启用：0-禁用，1-启用", example = "1")
    private Integer isEnabled;
}
