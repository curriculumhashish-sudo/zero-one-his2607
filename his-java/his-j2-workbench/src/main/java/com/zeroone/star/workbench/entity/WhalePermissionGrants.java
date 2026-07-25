package com.zeroone.star.workbench.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 权限授权
 * </p>
 *
 * @author 鹿
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("whale_permission_grants")
public class WhalePermissionGrants implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 权限名称
     */
    private String permissionName;

    /**
     * 授权主体类型（R=角色/U=用户）
     */
    private String providerName;

    /**
     * 授权主体键
     */
    private String providerKey;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

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
