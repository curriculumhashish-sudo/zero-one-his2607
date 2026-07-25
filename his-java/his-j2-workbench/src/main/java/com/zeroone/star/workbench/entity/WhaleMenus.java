package com.zeroone.star.workbench.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 菜单
 * </p>
 *
 * @author 鹿
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("whale_menus")
public class WhaleMenus implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableField(fill = FieldFill.INSERT)
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
    private Boolean isEnabled;

    /**
     * 是否隐藏
     */
    private Boolean isHidden;

    /**
     * 是否外链
     */
    private Boolean isExternal;

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
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;

    /**
     * 是否缓存页面
     */
    private Boolean keepAlive;
}
