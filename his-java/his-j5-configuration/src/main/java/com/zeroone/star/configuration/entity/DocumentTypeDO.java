package com.zeroone.star.configuration.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 描述：文书类型实体
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zeroone
 * @version 1.0.0
 */
@Data
@TableName("emr_document_type")
public class DocumentTypeDO implements Serializable {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;

    @TableField("tenant_id")
    private String tenantId;

    @TableField(value = "created_at", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(value = "updated_at", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField("created_by")
    private String createdBy;

    @TableField("updated_by")
    private String updatedBy;

    @TableField("is_deleted")
    @TableLogic
    private Integer isDeleted;

    @TableField("deleted_at")
    private LocalDateTime deletedAt;

    @TableField("deleted_by")
    private String deletedBy;

    /**
     * 文书类型编码
     */
    private String code;

    /**
     * 文书类型名称
     */
    private String name;

    /**
     * 默认模板ID
     */
    private String defaultTemplateId;

    /**
     * 是否启用
     */
    private Integer isEnabled;

    /**
     * 排序号
     */
    private Integer sortOrder;
}
