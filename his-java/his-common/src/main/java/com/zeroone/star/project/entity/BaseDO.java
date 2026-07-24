package com.zeroone.star.project.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * 所有实体DO的父类，抽离出了公共字段
 * @author 是丫丫
 * @date 2026/7/24
 */
@Getter
@Setter
@ToString
public abstract class BaseDO implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键id，VARCHAR(64) 雪花ID/自定义字符串ID
     */
    @TableId(type = IdType.INPUT)
    @TableField(fill = FieldFill.INSERT)
    private String id;

    /**
     * 创建时间，DATETIME(6) 自动填充
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 更新时间，新增、修改都自动填充
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人ID
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 更新人ID
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;
}
