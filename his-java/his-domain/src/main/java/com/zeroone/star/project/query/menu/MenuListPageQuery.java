package com.zeroone.star.project.query.menu;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

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

    @Schema(description = "排序方向", example = "asc")
    private String sortDirection;

    @Schema(description = "页码", example = "1")
    private Integer pageNo;

    @Schema(description = "是否启用", example = "true")
    private Boolean isEnabled;

    @Schema(description = "菜单类型", example = "M")
    private String menuType;

    @Schema(description = "排序字段", example = "sortOrder")
    private String sortBy;

    @Schema(description = "高级查询条件")
    private List<Object> where;

    @Schema(description = "搜索关键字", example = "系统")
    private String searchKey;

    @Schema(description = "关键字", example = "system")
    private String keyword;

    @Schema(description = "父菜单ID", example = "0")
    private Long parentId;

    @Schema(description = "是否隐藏", example = "false")
    private Boolean isHidden;

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
