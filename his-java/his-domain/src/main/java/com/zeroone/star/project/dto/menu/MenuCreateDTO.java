package com.zeroone.star.project.dto.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：菜单新增传输数据
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author osillow
 * @version 1.0.0
 * @date 2026/07/18 21:00
 */
@Data
@Schema(description = "菜单新增传输数据对象")
public class MenuCreateDTO {

    /**
     * 父菜单ID，0为一级目录
     */
    @Schema(description = "父菜单ID，0为一级目录", example = "0")
    private Long parentId;

    @Schema(description = "外部链接地址", example = "https://example.com")
    private String externalUrl;

    @Schema(description = "菜单编码", example = "system", requiredMode = Schema.RequiredMode.REQUIRED)
    private String code;

    @Schema(description = "是否缓存页面", example = "true")
    private Boolean keepAlive;

    @Schema(description = "是否外链", example = "false")
    private Boolean isExternal;

    @Schema(description = "显示名称", example = "系统管理")
    private String displayName;

    @Schema(description = "备注", example = "系统管理目录")
    private String remark;

    @Schema(description = "是否隐藏", example = "false")
    private Boolean isHidden;

    @Schema(description = "排序号", example = "1")
    private Integer sortOrder;

    @Schema(description = "是否启用", example = "true")
    private Boolean isEnabled;

    @Schema(description = "菜单名称", example = "system", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    @Schema(description = "菜单类型", example = "M")
    private String menuType;

    @Schema(description = "权限名称", example = "system:menu:list")
    private String permissionName;

    /**
     * 菜单英文名称
     */
    @Schema(description = "菜单英文名称", example = "system")
    private String menuName;

    /**
     * 页面显示中文名称
     */
    @Schema(description = "页面显示中文名称", example = "系统管理")
    private String showName;

    /**
     * 前端路由路径
     */
    @Schema(description = "前端路由路径", example = "/system")
    private String path;

    /**
     * 前端vue组件地址
     */
    @Schema(description = "前端vue组件地址", example = "system/menu/index")
    private String component;

    /**
     * 权限标识
     */
    @Schema(description = "权限标识", example = "system:menu:list")
    private String perms;

    /**
     * 菜单图标
     */
    @Schema(description = "菜单图标", example = "dashboard")
    private String icon;

    /**
     * 排序号
     */
    @Schema(description = "排序号", example = "1")
    private Integer orderNum;
}
