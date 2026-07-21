package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.FieldStrategy;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * 病历模板实体，对应 emr_template 表。
 */
@Getter
@Setter
@TableName("emr_template")
public class MedicalTemplate implements Serializable {

    private static final long serialVersionUID = 1L;

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    private String name;

    private String code;

    private String documentTypeCode;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String organizationId;

    private Integer encounterClassEnum;

    private Integer encounterTypeEnum;

    private String content;

    @TableField("is_enabled")
    private Boolean enabled;

    private String scopeLevel;

    @TableField(updateStrategy = FieldStrategy.ALWAYS)
    private String ownerPractitionerId;

    private String encounterClassCode;

    private LocalDateTime createdAt;

    private LocalDateTime updatedAt;

    private String createdBy;

    private String updatedBy;

    @TableLogic(value = "0", delval = "1")
    @TableField("is_deleted")
    private Boolean deleted;

    private LocalDateTime deletedAt;

    private String deletedBy;

    private String tenantId;
}
