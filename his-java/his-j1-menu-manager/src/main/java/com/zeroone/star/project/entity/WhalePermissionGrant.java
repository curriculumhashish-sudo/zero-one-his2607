package com.zeroone.star.project.entity;

import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * whale_permission_grants 表实体
 * @author CatSN
 */
@Data
@TableName("whale_permission_grants")
public class WhalePermissionGrant {
    @TableId
    private String id;
    private String permissionName;
    private String providerName;
    private String providerKey;
    private String tenantId;
    private LocalDateTime createdAt;
    private String createdBy;
    private Integer isDeleted;
}
