package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.*;
import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serial;
import java.io.Serializable;
import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * T2 排班日模板（07 §5.3）
 * </p>
 *
 * @author 墨
 * @since 2026-07-21
 */
@Getter
@Setter
@ToString
@TableName("`bkg_schedule_template`")
public class BkgScheduleTemplate implements Serializable {

    @Serial
    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_UUID)
    private String id;

    /**
     * 租户标识
     */
    @TableField("`tenant_id`")
    private String tenantId;

    /**
     * 机构/科室；空=全院
     */
    @TableField("`organization_id`")
    private String organizationId;

    /**
     * 模板编码
     */
    @TableField("`template_code`")
    private String templateCode;

    /**
     * 模板名称
     */
    @TableField("`template_name`")
    private String templateName;

    /**
     * 排班模式：MODE-A / MODE-B / MODE-C
     */
    @TableField("`schedule_mode`")
    private String scheduleMode;

    /**
     * 默认医师主键（可选）
     */
    @TableField("`practitioner_id`")
    private String practitionerId;

    /**
     * 默认诊室主键（可选）
     */
    @TableField("`location_id`")
    private String locationId;

    /**
     * 号源类型主键
     */
    @TableField("`healthcare_service_id`")
    private String healthcareServiceId;

    /**
     * 关联 T1 放号时段模板
     */
    @TableField("`slot_provision_template_id`")
    private String slotProvisionTemplateId;

    /**
     * 默认放号策略主键
     */
    @TableField("`default_release_policy_id`")
    private String defaultReleasePolicyId;

    /**
     * 生效日期起
     */
    @JsonFormat(pattern = "yyyy-MM-dd")
    private LocalDate effectiveStart;

    /**
     * 生效日期止
     */
    @JsonFormat(pattern = "yyyy-MM-dd")
    private LocalDate effectiveEnd;

    /**
     * 匹配优先级（数值越大越优先）
     */
    @TableField("`priority`")
    private Integer priority;

    /**
     * 来源岗位主键（导入追溯）
     */
    @TableField("`source_practitioner_role_id`")
    private String sourcePractitionerRoleId;

    /**
     * 是否启用
     */
    @TableField("`enabled`")
    private Boolean enabled;

    /**
     * 创建时间
     */
    @TableField(value = "`created_at`", fill = FieldFill.INSERT)
    private LocalDateTime createdAt;

    /**
     * 最后更新时间
     */
    @TableField(value = "`updated_at`", fill = FieldFill.INSERT_UPDATE)
    private LocalDateTime updatedAt;

    /**
     * 创建人
     */
    @TableField(value = "`created_by`", fill = FieldFill.INSERT)
    private String createdBy;

    /**
     * 最后更新人
     */
    @TableField(value = "`updated_by`", fill = FieldFill.INSERT_UPDATE)
    private String updatedBy;

    /**
     * 软删标记
     */
    @TableLogic
    @TableField(value = "`is_deleted`")
    private Boolean isDeleted;

    /**
     * 删除时间
     */
    @TableField(value = "`deleted_at`")
    private LocalDateTime deletedAt;

    /**
     * 删除人
     */
    @TableField(value = "`deleted_by`")
    private String deletedBy;

    /**
     * 乐观锁版本号
     */
    @Version
    @TableField(value = "`version`")
    private Integer version;
}
