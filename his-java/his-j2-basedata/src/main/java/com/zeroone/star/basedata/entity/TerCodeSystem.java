package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@TableName("ter_code_system")
public class TerCodeSystem {
    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    private String name;
    private String title;
    private String description;
    private String version;
    private String status;
    private String tenantId;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    private String createdBy;
    private String updatedBy;
    private Integer isDeleted;
    private LocalDateTime deletedAt;
    private String deletedBy;
}
