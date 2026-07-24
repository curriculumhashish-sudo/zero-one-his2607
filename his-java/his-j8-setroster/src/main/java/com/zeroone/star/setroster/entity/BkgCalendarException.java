package com.zeroone.star.setroster.entity;

import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * T3 日历例外（07 §5.4）
 * </p>
 *
 * @author lolololo
 * @since 2026-07-23
 */
@Getter
@Setter
@ToString
@TableName("bkg_calendar_exception")
public class BkgCalendarException implements Serializable {

    private static final long serialVersionUID = 1L;

    /**
     * 主键
     */
    @TableId(value = "id", type = IdType.ASSIGN_ID)
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
     * 从模板生成 排班 时是否自动应用
     */
    private Boolean autoApplyOnGenerate;

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
