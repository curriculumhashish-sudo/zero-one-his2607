package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 文书类型实体类
 */
@Data
@TableName("emr_document_type")
public class DocumentTypeDO {
    
    @TableId(type = IdType.ASSIGN_ID)
    private String id;
    
    @TableField("code")
    private String code;
    
    @TableField("name")
    private String name;
    
    @TableField("default_template_id")
    private String defaultTemplateId;
    
    @TableField("is_enabled")
    private Boolean isEnabled;
    
    @TableField("sort_order")
    private Integer sortOrder;
    
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;
    
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;
    
    @TableField("created_by")
    private String createdBy;
    
    @TableField("updated_by")
    private String updatedBy;
    
    @TableLogic
    @TableField("is_deleted")
    private Boolean isDeleted;
    
    @TableField("deleted_at")
    private LocalDateTime deletedAt;
    
    @TableField("deleted_by")
    private String deletedBy;
    
    @TableField("tenant_id")
    private String tenantId;
}
