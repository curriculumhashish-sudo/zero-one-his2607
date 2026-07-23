package com.zeroone.star.sysmgr.entity.menu;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import java.util.Date;

/**
 * 菜单实体DO
 * 对应数据库表 whale_menus
 * @author Drums小白
 * @date 2026/07/23
 */
@Data
@TableName("whale_menus")
public class MenuDO {

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_ID)
    private String id;

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
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private Date createdAt;

    /**
     * 最后更新时间
     */
    private Date updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Integer isDeleted;

    /**
     * 删除时间
     */
    private Date deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 是否缓存页面
     */
    private Integer keepAlive;
}
