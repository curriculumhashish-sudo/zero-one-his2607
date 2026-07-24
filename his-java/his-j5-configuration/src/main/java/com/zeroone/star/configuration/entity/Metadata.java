package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * 元数据字段实体类，映射数据库表 {@code emr_metadata_field_def}。
 * <p>
 * 用于管理 HIS 系统中可配置的业务字段元信息，包括字段标识、显示名称、
 * 取值来源、编辑类型、字段类型与长度精度约束等。支持逻辑删除和自动填充
 * 创建/更新人及时间。
 * </p>
 */
@Getter
@Setter
@ToString
@TableName("`emr_metadata_field_def`")
public class Metadata implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 ID，使用雪花算法自动生成 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 字段标识，唯一标识一个字段 */
    @TableField("field_key")
    private String fieldKey;

    /** 字段显示标签 */
    @TableField("label")
    private String label;

    /** 数据类型 */
    @TableField("data_type")
    private String dataType;

    /** 默认可编辑 */
    @TableField("editable_default")
    private Integer editableDefault;

    /** 字段取值来源 */
    @TableField("value_source")
    private String valueSource;

    /** 取值来源路径 */
    @TableField("source_path")
    private String sourcePath;

    /** 取值来源配置（JSON 文本） */
    @TableField("source_config")
    private String sourceConfig;

    /** 默认值 */
    @TableField("default_value")
    private String defaultValue;

    /** 脱敏规则 */
    @TableField("mask_rule")
    private String maskRule;

    /** 格式模式 */
    @TableField("format_pattern")
    private String formatPattern;

    /** 字段排序顺序 */
    @TableField("sort_order")
    private Integer sortOrder;

    /** 是否启用 */
    @TableField("is_enabled")
    private Integer isEnabled;

    /** 备注 */
    @TableField("remark")
    private String remark;

    /** 创建人 */
    @TableField(value = "created_by", fill = FieldFill.INSERT)
    private String createdBy;

    /** 创建时间 */
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /** 更新人 */
    @TableField(value = "updated_by", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /** 更新时间 */
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /** 是否删除（逻辑删除） */
    @TableField("is_deleted")
    @TableLogic
    private Integer isDeleted;

    /** 删除时间 */
    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    /** 删除人 */
    @TableField("deleted_by")
    private String deletedBy;

    /** 租户标识 */
    @TableField("tenant_id")
    private String tenantId;
}
