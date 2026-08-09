package com.zeroone.star.basedata.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 字典项表
 * </p>
 *
 * @author xingluo
 */
@Getter
@Setter
@TableName("ter_concept")
public class TerConcept implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键ID
     */
    @TableId(value = "id", type = IdType.AUTO)
    private Long id;

    /**
     * 代码体系ID
     */
    private Long codeSystemId;

    /**
     * 字典项编码
     */
    private String code;

    /**
     * 显示名称
     */
    private String display;

    /**
     * 定义描述
     */
    private String definition;

    /**
     * 父级ID
     */
    private Long parentId;

    /**
     * 排序号
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
     * 租户ID
     */
    private Long tenantId;

    /**
     * 创建时间
     */
    private LocalDateTime createdAt;

    /**
     * 更新时间
     */
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    private Long createdBy;

    /**
     * 更新人
     */
    private Long updatedBy;

    /**
     * 逻辑删除标记
     */
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    private Long deletedBy;

}
