package com.zeroone.star.sysmgr.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 租户
 * </p>
 *
 * @author 鹿
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("whale_tenants")
public class WhaleTenants implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 租户名称
     */
    private String name;

    /**
     * 租户代码
     */
    private String code;

    /**
     * 租户描述
     */
    private String description;

    /**
     * 是否启用
     */
    private Boolean isActive;

    /**
     * 扩展属性
     */
    private String extraProperties;

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
