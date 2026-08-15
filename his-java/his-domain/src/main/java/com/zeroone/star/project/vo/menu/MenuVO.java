package com.zeroone.star.project.vo.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 菜单管理分页树形VO
 * @author Drums小白
 * @date 2026/07/23 21:20
 */
@Data
@Schema(description = "菜单信息")
public class MenuVO {

    @Schema(description = "菜单id")
    private String menuId;

    @Schema(description = "菜单英文标识")
    private String menuName;

    @Schema(description = "菜单显示名称")
    private String showName;

    @Schema(description = "菜单图标")
    private String icon;

    @Schema(description = "前端路由地址")
    private String path;

    @Schema(description = "前端组件路径")
    private String component;

    @Schema(description = "父菜单id")
    private String parentId;

    @Schema(description = "序号")
    private Long sortNum;

    @Schema(description = "菜单类型")
    private String menuType;

    @Schema(description = "启用状态 0启用 1停用")
    private String status;
}
