package com.zeroone.star.j7.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 编码系统
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
@Getter
@Setter
@ToString
@TableName("ter_code_system")
public class TerCodeSystem implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 编码系统
     */
    private String name;

    /**
     * 编码系统标题
     */
    private String title;

    /**
     * 描述
     */
    private String description;

    /**
     * 乐观锁版本号
     */
    private String version;

    /**
     * 状态：draft
     */
    private String status;

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
