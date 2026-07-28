package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * 元数据字段实体类，映射数据库表 {@code metadata_field}。
 * <p>
 * 用于管理 HIS 系统中可配置的业务字段元信息，包括字段标识、显示名称、
 * 取值来源、编辑类型、字段类型与长度精度约束等。支持逻辑删除和自动填充
 * 创建/更新人及时间。
 * </p>
 */
@Getter
@Setter
@ToString
@TableName("`metadata_field`")
public class Metadata implements Serializable {

    private static final long serialVersionUID = 1L;

    /** 主键 ID，使用雪花算法自动生成 */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    /** 字段标识，唯一标识一个字段 */
    @TableField("field_key")
    private String fieldKey;

    /** 字段显示名称 */
    @TableField("display_name")
    private String displayName;

    /** 字段取值来源 */
    @TableField("value_source")
    private String valueSource;

    /** 字段编辑类型 */
    @TableField("edit_type")
    private String editType;

    /** 字段类型 */
    @TableField("field_type")
    private String fieldType;

    /** 字段长度 */
    @TableField("length")
    private Integer length;

    /** 字段精度 */
    @TableField("precision")
    private Integer precision;

    /** 是否可为空 */
    @TableField("nullable")
    private Boolean nullable;

    /** 默认值 */
    @TableField("default_value")
    private String defaultValue;

    /** 字段描述 */
    @TableField("description")
    private String description;

    /** 字段排序顺序 */
    @TableField("sort_order")
    private Integer sortOrder;

    /** 字段状态 */
    @TableField("status")
    private Integer status;

    /** 创建人 */
    @TableField(fill = FieldFill.INSERT)
    private String createBy;

    /** 创建时间 */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createTime;

    /** 更新人 */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updateBy;

    /** 更新时间 */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updateTime;

    /** 是否删除 */
    @TableField("deleted")
    @TableLogic
    private Integer deleted;
}