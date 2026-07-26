package com.zeroone.star.sysmgr.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("whale_settings")
public class WhaleSettings {
    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    private String name;
    private String value;
    private String providerName;
    private String providerKey;
    private String tenantId;
    private String description;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private Integer isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;

}
