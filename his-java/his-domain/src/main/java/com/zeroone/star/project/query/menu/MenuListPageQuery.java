package com.zeroone.star.project.query.menu;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Min;
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

    /**
     * 菜单名称，支持模糊搜索
     */
    @Schema(description = "菜单名称", example = "系统管理")
    private String menuName;

    /**
     * 状态：0启用 1停用
     */
    @Schema(description = "状态：0启用 1停用", example = "0")
    private String status;

}
