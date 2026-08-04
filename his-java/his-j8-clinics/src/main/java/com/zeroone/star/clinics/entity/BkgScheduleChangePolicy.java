package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 停诊与预约处置院点策略（07 §4.2）
 * </p>
 *
 * @author aike
 * @since 2026-08-01
 */
@Getter
@Setter
@ToString
@TableName("bkg_schedule_change_policy")
public class BkgScheduleChangePolicy implements Serializable {

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
     * 机构标识；空=租户默认
     */
    private String organizationId;

    /**
     * 停诊默认处置：TRANSFER / RESCHEDULE / CANCEL / MANUAL
     */
    private String onPractitionerStopMode;

    /**
     * 转交匹配规则
     */
    private String transferMatchRule;

    /**
     * 转交是否要求同号源类型
     */
    private Boolean transferRequireSameService;

    /**
     * 是否允许跨医师转交
     */
    private Boolean transferAllowCrossPractitioner;

    /**
     * 转交患者确认：NONE / SMS_CONFIRM / WINDOW_CONFIRM
     */
    private String transferPatientConfirmMode;

    /**
     * 存在未处置预约时禁止完成停诊
     */
    private Boolean blockDeactivateIfUnhandled;

    /**
     * 转交后是否通知患者
     */
    private Boolean notifyOnTransfer;

    /**
     * 停诊取消预约时是否通知
     */
    private Boolean notifyOnCancelStop;

    /**
     * 停诊预览后冻结源号源，防并发新约
     */
    private Boolean freezeSourceSlotOnPreview;

    /**
     * 冻结超时分钟数（TTL 自动释放）
     */
    private Integer freezeTtlMinutes;

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
