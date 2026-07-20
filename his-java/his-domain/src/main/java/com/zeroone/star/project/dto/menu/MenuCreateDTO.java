package com.zeroone.star.project.dto.menu;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
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
    @NotNull(message = "父菜单ID不能为空")
    @Schema(description = "父菜单ID，0为一级目录", example = "0", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long parentId;

    /**
     * 菜单英文名称
     */
    @NotBlank(message = "菜单名称不能为空")
    @Schema(description = "菜单英文名称", example = "system", requiredMode = Schema.RequiredMode.REQUIRED)
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
