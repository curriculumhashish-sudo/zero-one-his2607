package com.zeroone.star.project.vo.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 菜单管理分页树形
 * @author Drums小白
 * @date 2026/07/18 21:00
 */
@Data
@Schema(description = "菜单管理分页树形")
public class MenuVO {

    @Schema(description = "序号", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long sortNum;

    @Schema(description = "菜单英文名称", requiredMode = Schema.RequiredMode.REQUIRED)
    private String menuName;

    @Schema(description = "页面显示中文名称", requiredMode = Schema.RequiredMode.REQUIRED)
    private String showName;

    @Schema(description = "菜单图标", requiredMode = Schema.RequiredMode.REQUIRED)
    private String icon;

    @Schema(description = "前端路由路径", requiredMode = Schema.RequiredMode.REQUIRED)
    private String path;

    @Schema(description = "前端vue组件地址", requiredMode = Schema.RequiredMode.REQUIRED)
    private String component;

    @Schema(description = "排序号", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer sort;

    @Schema(description = "状态：0启用 1停用", requiredMode = Schema.RequiredMode.REQUIRED)
    private String status;

    @Schema(description = "菜单ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long menuId;

    @Schema(description = "父菜单ID，0为一级目录", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long parentId;

    @Schema(description = "菜单类型：C目录 M菜单 F按钮", requiredMode = Schema.RequiredMode.REQUIRED)
    private String menuType;
}