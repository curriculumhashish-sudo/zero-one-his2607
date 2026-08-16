package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 放号策略：提前天数、时段粒度、每段容量、渠道配额
 * </p>
 *
 * @author Zzen
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("bkg_release_policy")
public class ReleasePolicy implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;

    /**
     * 租户标识
     */
    private String tenantId;

    /**
     * 机构标识；空=租户默认
     */
    private String organizationId;

    /**
     * 策略编码（机构内唯一）
     */
    private String policyCode;

    /**
     * 策略名称
     */
    private String policyName;

    /**
     * 可预约提前天数
     */
    private Integer advanceDays;

    /**
     * 默认时段粒度（分钟）
     */
    private Integer slotMinutes;

    /**
     * 默认每时段容量
     */
    private Integer capacityPerSlot;

    /**
     * 渠道配额 JSON，如 {"WINDOW":60,"APP":40}
     */
    private String channelQuotaJson;

    /**
     * 创建时间
     */
    @TableField(fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
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
    @Version
    private Integer version;
}
