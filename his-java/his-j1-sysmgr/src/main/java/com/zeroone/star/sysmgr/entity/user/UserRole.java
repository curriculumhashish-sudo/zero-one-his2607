package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;

import com.zeroone.star.project.entity.BaseDO;
import lombok.*;

/**
 * <p>
 * 用户角色关联
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("whale_user_roles")
public class UserRole extends BaseDO {

    /**
     * 用户标识
     */
    @TableField("user_id")
    private String userId;

    /**
     * 角色标识
     */
    @TableField("role_id")
    private String roleId;

    /**
     * 租户标识
     */
    @TableField("tenant_id")
    private String tenantId;

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
