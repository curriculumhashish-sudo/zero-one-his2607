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
 * TW 周排班模板
 * </p>
 *
 * @author Zzen
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("bkg_week_schedule_template")
public class WeekScheduleTemplate implements Serializable {

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
     * 无排班时模板态是否允许预约（虚拟可约）
     */
    private Boolean fallbackBookable;

    /**
     * 科室默认周模板（同科室仅一条 is_默认=true）
     */
    private Boolean isDefault;

    /**
     * 是否启用
     */
    private Boolean enabled;

    /**
     * 生效日期起
     */
    private LocalDate effectiveStart;

    /**
     * 生效日期止
     */
    private LocalDate effectiveEnd;

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
