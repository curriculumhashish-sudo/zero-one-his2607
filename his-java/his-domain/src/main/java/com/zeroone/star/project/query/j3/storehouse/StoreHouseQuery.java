package com.zeroone.star.project.query.j3.storehouse;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;

/**
 * <p>
 * 描述：仓库查询参数
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author mie
 * @version 1.0.0
 */
@Getter
@Setter
public class StoreHouseQuery extends PageQuery {

    @Schema(description = "关键字（名称模糊搜索）", example = "药")
    private String keyword;

    @Schema(description = "仓库类型ID（1仓库 2药房 3科室药房）", example = "1")
    private Long type;


}
