package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.FieldFill;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableLogic;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;

import java.time.LocalDateTime;

/**
 * 字典项数据对象，对应 ter_concept 表。
 */
@Data
@TableName("ter_concept")
public class DictionaryConceptDO {

    @TableId(value = "id", type = IdType.ASSIGN_ID)
    private String id;
    private String codeSystemId;
    private String code;
    private String display;
    private String definition;
    private String parentId;
    private Long sortOrder;
    private Boolean isActive;
    private String cssClass;
    private String listClass;
    private String isDefault;
    private String tenantId;

    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    @TableLogic(value = "0", delval = "1")
    private Boolean isDeleted;

    @TableField(fill = FieldFill.UPDATE)
    private LocalDateTime deletedAt;

    @TableField(fill = FieldFill.UPDATE)
    private String deletedBy;
}
