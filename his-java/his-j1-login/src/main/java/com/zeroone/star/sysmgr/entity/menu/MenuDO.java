package com.zeroone.star.sysmgr.entity.menu;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 菜单实体，映射 whale_menus 表。
 */
@Data
@TableName("whale_menus")
public class MenuDO {

    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    /** 菜单编码 */
    private String code;

    /** 菜单名称 */
    private String name;

    /** 显示名称 */
    private String displayName;

    /** 图标 */
    private String icon;

    /** 路由路径 */
    private String path;

    /** 前端组件路径 */
    private String component;

    /** 关联权限名称 */
    private String permissionName;

    /** 父菜单 ID */
    private String parentId;

    /** 排序号 */
    private Integer sortOrder;

    /** 是否启用 1启用 0禁用 */
    private Integer isEnabled;

    /** 是否隐藏 1隐藏 0显示 */
    private Integer isHidden;

    /** 租户ID */
    private String tenantId;

    /** 创建时间 */
    private LocalDateTime createdAt;

    /** 更新时间 */
    private LocalDateTime updatedAt;

    @TableLogic(value = "0", delval = "1")
    private Integer isDeleted;
}
