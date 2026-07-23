package com.zerone.star.project.entity.menu;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.time.LocalDateTime;

/**
 * 菜单实体DO
 * 对应数据库表 whale_menus
 * @author Drums小白
 * @date 2026/07/23
 */
@Data
@TableName("whale_menus")
@Schema(description = "菜单信息")
public class MenuDO {

    @TableId(type = IdType.AUTO)
    @Schema(description = "菜单id")
    private Long menuId;

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

    @Schema(description = "前端导航标识")
    private String navigationKey;

    @Schema(description = "父菜单id")
    private Long parentId;

    @Schema(description = "排序")
    private Integer sort;

    @Schema(description = "是否可见 1可见 0不可见")
    private Integer visible;

    @Schema(description = "页面是否缓存 1缓存 0不缓存")
    private Integer cache;

    @Schema(description = "侧边栏隐藏标识 1隐藏 0显示")
    private Integer hidden;

    @Schema(description = "路由重定向地址")
    private String redirect;

    @Schema(description = "菜单类型")
    private String menuType;

    @Schema(description = "备注")
    private String remark;

    @Schema(description = "租户id")
    private Long tenantId;

    @Schema(description = "创建时间")
    private LocalDateTime createTime;

    @Schema(description = "更新时间")
    private LocalDateTime updateTime;

    @Schema(description = "启用状态 1启用 0停用")
    private Integer enabled;

    @Schema(description = "逻辑删除标识 0正常 1已删除")
    private Integer deleted;

    @Schema(description = "归档标识")
    private Integer isArchive;

    @Schema(description = "归档时间")
    private LocalDateTime archiveTime;

    @Schema(description = "扩展信息")
    private String extInfo;

    @Schema(description = "序号")
    private Long sortNum;
}
