package com.zeroone.star.sysmgr.entity.menu;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.io.Serializable;

/**
 * 登录菜单实体，对应 whale_menus 表
 * 仅包含菜单树展示所需的字段
 */
@Data
@TableName("whale_menus")
public class LoginMenuDO implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 菜单ID */
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
    /** 父菜单ID */
    private String parentId;
    /** 排序号 */
    private Integer sortOrder;
    /** 是否启用 */
    private Integer isEnabled;
    /** 是否隐藏 */
    private Integer isHidden;
    /** 菜单类型 */
    private String menuType;

    // ---- 继承自 BaseDO 的审计字段 ----
    private String createdAt;
    private String updatedAt;
    private Integer isDeleted;
}
