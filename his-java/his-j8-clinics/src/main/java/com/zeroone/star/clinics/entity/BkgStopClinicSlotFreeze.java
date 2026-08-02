package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 停诊预览期间源号源冻结（防并发新约）
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("bkg_stop_clinic_slot_freeze")
public class BkgStopClinicSlotFreeze implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 机构标识
     */
    private String organizationId;

    /**
     * 被停诊日程主键
     */
    private String scheduleId;

    /**
     * 被冻结的源号源主键
     */
    private String slotId;

    /**
     * 冻结过期时间（TTL 自动释放）
     */
    private LocalDateTime expiresAt;

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

    /**
     * 乐观锁版本号
     */
    private Integer version;
}
