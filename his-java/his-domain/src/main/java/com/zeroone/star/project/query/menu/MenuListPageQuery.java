package com.zeroone.star.project.query.menu;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：菜单列表分页查询数据,支持条件和分页
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author osillow
 * @version 1.0.0
 * @date 2026/07/18 21:00
 */
@Data
@Schema(description = "菜单列表分页查询数据对象")
public class MenuListPageQuery extends PageQuery {

    @Schema(description = "关键字", example = "system")
    private String keyword;
}
