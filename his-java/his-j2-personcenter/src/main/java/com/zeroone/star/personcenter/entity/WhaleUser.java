package com.zeroone.star.personcenter.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * <p>
 * 描述：系统用户实体，映射 whale_users 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@TableName("whale_users")
public class WhaleUser {

    @TableId(value = "user_id", type = IdType.ASSIGN_ID)
    private Long userId;

    private Long tenantId;

    private String userName;

    private String realName;

    private String avatar;

    private String email;

    private String phoneNumber;

    private String introduction;

    private String passwordHash;

    private Boolean isActive;

    private Boolean isLockedOut;

    private LocalDateTime lockoutEnd;

    private Integer accessFailedCount;

    private Long organizationId;

    private Long practitionerId;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic
    private Boolean isDeleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    @Version
    private Integer version;
}
