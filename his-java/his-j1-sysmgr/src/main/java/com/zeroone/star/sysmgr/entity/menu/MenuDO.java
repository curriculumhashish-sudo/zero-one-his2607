package com.zeroone.star.sysmgr.entity.menu;

import com.baomidou.mybatisplus.annotation.TableName;
import com.zeroone.star.project.entity.BaseDO;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;

/**
 * 菜单实体DO
 * 对应数据库表 whale_menus
 * @author Drums小白
 * @date 2026/07/23
 */
@Getter
@Setter
@ToString
@TableName("whale_menus")
public class MenuDO extends BaseDO {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 菜单编码
     */
    private String code;

    /**
     * 菜单名称
     */
    private String name;

    /**
     * 显示名称
     */
    private String displayName;

    /**
     * 图标
     */
    private String icon;

    /**
     * 路由路径
     */
    private String path;

    /**
     * 前端组件路径
     */
    private String component;

    /**
     * 关联权限名称
     */
    private String permissionName;

    /**
     * 父菜单 ID
     */
    private String parentId;

    /**
     * 排序号
     */
    private Integer sortOrder;

    /**
     * 是否启用
     */
    private Integer isEnabled;

    /**
     * 是否隐藏
     */
    private Integer isHidden;

    /**
     * 是否外链
     */
    private Integer isExternal;

    /**
     * 外链 URL
     */
    private String externalUrl;

    /**
     * 菜单类型（menu/directory）
     */
    private String menuType;

    /**
     * 备注
     */
    private String remark;

    /**
     * 是否缓存页面
     */
    private Integer keepAlive;
}
