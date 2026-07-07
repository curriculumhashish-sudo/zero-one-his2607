package com.zeroone.star.sysmgr.entity.permission;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@ToString
@TableName("whale_permissions")
public class Permission implements Serializable {
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;
    private String name;
    private String displayName;
    private String parentName;
    private String description;
    private String tenantId;
    private Boolean isDeleted;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private LocalDateTime deletedAt;
    private String deletedBy;
}