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
 * 用户角色关联
 * </p>
 *
 * @author 鹿
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_user_roles")
public class WhaleUserRoles implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableField(fill = FieldFill.INSERT)
    private String id;

    /**
     * 用户标识
     */
    private String userId;

    /**
     * 角色标识
     */
    private String roleId;

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
}
