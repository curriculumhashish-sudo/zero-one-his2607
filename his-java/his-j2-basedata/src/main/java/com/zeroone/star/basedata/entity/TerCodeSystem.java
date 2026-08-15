package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.*;
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

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    private Integer isDeleted;

    @TableField(fill = FieldFill.UPDATE)
    private LocalDateTime deletedAt;

    @TableField(fill = FieldFill.UPDATE)
    private String deletedBy;

}
