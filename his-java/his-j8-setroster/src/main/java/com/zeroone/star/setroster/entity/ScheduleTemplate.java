package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.*;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * T2 排班日模板
 * </p>
 *
 * @author Zzen
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("bkg_schedule_template")
public class ScheduleTemplate implements Serializable {

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
     * 机构/科室；空=全院
     */
    private String organizationId;

    /**
     * 模板编码
     */
    private String templateCode;

    /**
     * 模板名称
     */
    private String templateName;

    /**
     * 排班模式：MODE-A / MODE-B / MODE-C
     */
    private String scheduleMode;

    /**
     * 默认医师主键（可选）
     */
    private String practitionerId;

    /**
     * 默认诊室主键（可选）
     */
    private String locationId;

    /**
     * 号源类型主键
     */
    private String healthcareServiceId;

    /**
     * 关联 T1 放号时段模板
     */
    private String slotProvisionTemplateId;

    /**
     * 默认放号策略主键
     */
    private String defaultReleasePolicyId;

    /**
     * 生效日期起
     */
    private LocalDate effectiveStart;

    /**
     * 生效日期止
     */
    private LocalDate effectiveEnd;

    /**
     * 匹配优先级（数值越大越优先）
     */
    private Integer priority;

    /**
     * 来源岗位主键（导入追溯）
     */
    private String sourcePractitionerRoleId;

    /**
     * 是否启用
     */
    private Boolean enabled;

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
