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
 * 日历例外
 * </p>
 *
 * @author Zzen
 * @since 2026-07-24
 */
@Getter
@Setter
@ToString
@TableName("bkg_calendar_exception")
public class CalendarException implements Serializable {

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
     * 例外日期或区间起点
     */
    private LocalDate exceptionDate;

    /**
     * 区间终点；空=单日
     */
    private LocalDate rangeEnd;

    /**
     * 类型：CLOSED / REDUCED / OVERRIDE / EXTRA
     */
    private String exceptionType;

    /**
     * REDUCED/OVERRIDE/EXTRA 时关联 T2
     */
    private String overrideScheduleTemplateId;

    /**
     * 原因说明
     */
    private String reason;

    /**
     * 从模板生成排班时是否自动应用
     */
    private Boolean autoApplyOnGenerate;

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
