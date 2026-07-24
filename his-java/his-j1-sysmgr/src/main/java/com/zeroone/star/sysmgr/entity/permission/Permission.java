package com.zeroone.star.sysmgr.entity.permission;

import com.baomidou.mybatisplus.annotation.TableName;
import com.zeroone.star.project.entity.BaseDO;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

@Getter
@Setter
@ToString
@TableName("whale_permissions")
public class Permission extends BaseDO implements Serializable  {
    private String name;
    private String displayName;
    private String parentName;
    private String description;
    private String tenantId;
    private Boolean isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
}