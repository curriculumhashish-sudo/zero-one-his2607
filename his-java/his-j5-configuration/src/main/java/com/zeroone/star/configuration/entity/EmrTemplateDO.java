package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 电子病历模板实体类
 */
@Data
@TableName("emr_template")
public class EmrTemplateDO {

    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    @TableField("name")
    private String name;

    @TableField("code")
    private String code;

    @TableField("document_type_code")
    private String documentTypeCode;

    @TableField("organization_id")
    private String organizationId;

    @TableField("encounter_class_enum")
    private Integer encounterClassEnum;

    @TableField("encounter_type_enum")
    private Integer encounterTypeEnum;

    @TableField("content")
    private String content;

    @TableField("is_enabled")
    private Boolean isEnabled;

    @TableField("scope_level")
    private String scopeLevel;

    @TableField("owner_practitioner_id")
    private String ownerPractitionerId;

    @TableField("encounter_class_code")
    private String encounterClassCode;

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
