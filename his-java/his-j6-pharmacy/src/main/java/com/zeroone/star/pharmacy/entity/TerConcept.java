package com.zeroone.star.pharmacy.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 编码概念
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("ter_concept")
public class TerConcept implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 编码概念
     */
    private String codeSystemId;

    /**
     * 编码
     */
    private String code;

    /**
     * 显示名称
     */
    private String display;

    /**
     * 定义
     */
    private String definition;

    /**
     * parent标识
     */
    private String parentId;

    /**
     * 父概念（支持层级结构，可选） / private Concept parent
     */
    private Long sortOrder;

    /**
     * 是否启用
     */
    private Boolean isActive;

    /**
     * CSS样式类
     */
    private String cssClass;

    /**
     * 列表样式类
     */
    private String listClass;

    /**
     * 是否默认
     */
    private String isDefault;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private String createdBy;

    /**
     * 最后更新人
     */
    private String updatedBy;

    /**
     * 软删标记
     */
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private String deletedBy;
}
