package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 文书类型 DO
 * 对应表：emr_document_type
 */
@Data
@TableName("emr_document_type")
public class EmrDocumentTypeDO {

    /**
     * 主键ID
     * varchar(64)
     */
    @TableId(type = IdType.ASSIGN_ID)
    private String id;

    /**
     * 文书类型编码
     * varchar(32)
     */
    private String code;

    /**
     * 文书类型名称
     * varchar(64)
     */
    private String name;

    /**
     * 默认模板ID（关联emr_template主键id，允许null）
     * varchar(64)
     */
    private String defaultTemplateId;

    /**
     * 是否启用 0-禁用 1-启用
     * tinyint(1)
     */
    private Integer isEnabled;

    /**
     * 排序号
     * int
     */
    private Integer sortOrder;

    /**
     * 创建时间
     * datetime(6)
     */
    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 更新时间
     * datetime(6)
     */
    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人ID
     * varchar(64)
     */
    private String createdBy;

    /**
     * 更新人ID
     * varchar(64)
     */
    private String updatedBy;

    /**
     * 是否删除 0未删除 1已删除（软删除）
     * tinyint(1)
     */
    @TableLogic
    @TableField("is_deleted")
    private Integer isDeleted;

    /**
     * 删除时间
     * datetime(6)
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人ID
     * varchar(64)
     */
    private String deletedBy;

    /**
     * 租户ID
     * varchar(64)
     */
    private String tenantId;

}
