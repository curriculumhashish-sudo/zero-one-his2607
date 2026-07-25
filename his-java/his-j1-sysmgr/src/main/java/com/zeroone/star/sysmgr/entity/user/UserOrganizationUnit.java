package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;
import java.io.Serializable;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;
import lombok.experimental.Accessors;

/**
 * <p>
 * 用户组织单元关联
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("whale_user_organization_units")
public class UserOrganizationUnit extends BaseDO {
    /**
     * 用户标识
     */
    @TableField("user_id")
    private String userId;

    /**
     * 组织单元标识
     */
    @TableField("organization_unit_id")
    private String organizationUnitId;

    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;
    /**
     * 软删标记
     */
    @TableField("is_deleted")
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @TableField("deleted_by")
    private String deletedBy;
}
