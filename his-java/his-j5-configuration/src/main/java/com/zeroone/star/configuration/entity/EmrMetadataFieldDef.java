package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 元数据字段数据类型的实体类
 */
@Data
@TableName("emr_metadata_field_def")
public class EmrMetadataFieldDef {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String fieldKey;

    private String label;

    private String dataType;

    private Integer editableDefault;

    private String valueSource;

    private String sourcePath;

    private String sourceConfig;

    private String defaultValue;

    private String maskRule;

    private String formatPattern;

    private Integer sortOrder;

    private Integer isEnabled;

    private String remark;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;
    
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;
    
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;
    
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;
    
    @TableLogic
    private Integer isDeleted;

    private LocalDateTime deletedAt;
    
    private String deletedBy;
    
    @TableField(fill = FieldFill.INSERT)
    private String tenantId;

}
