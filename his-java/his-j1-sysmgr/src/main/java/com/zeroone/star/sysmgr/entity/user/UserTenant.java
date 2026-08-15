package com.zeroone.star.sysmgr.entity.user;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;

import java.time.LocalDateTime;
import com.zeroone.star.project.entity.BaseDO;
import lombok.*;

/**
 * <p>
 * 用户租户关联
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-27
 */
@Getter
@Setter
@ToString
@TableName("whale_user_tenants")
public class UserTenant extends BaseDO {

    /**
     * 用户标识
     */
    @TableField("user_id")
    private String userId;

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
