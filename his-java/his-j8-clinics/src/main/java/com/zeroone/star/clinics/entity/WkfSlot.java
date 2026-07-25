package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 号源时段（FHIR 时段 语义）
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("wkf_slot")
public class WkfSlot implements Serializable {

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
    private Long version;

    /**
     * 机构标识（冗余自 排班）
     */
    private String organizationId;

    /**
     * 所属日程标识
     */
    private String scheduleId;

    /**
     * 时段开始时刻
     */
    private LocalDateTime startTime;

    /**
     * 时段结束时刻
     */
    private LocalDateTime endTime;

    /**
     * 状态码（free/busy/busy-unavailable 等）
     */
    private String statusCode;

    /**
     * 额定容量
     */
    private Integer capacity;

    /**
     * 已预约计数
     */
    private Integer bookedCount;

    /**
     * 超售标记
     */
    private Boolean overbookedFlag;

    /**
     * 服务大类快照 JSON
     */
    private String serviceCategoryJson;

    /**
     * 服务类型快照 JSON
     */
    private String serviceTypeJson;

    /**
     * 专科快照 JSON
     */
    private String specialtyJson;

    /**
     * 预约类型/场景快照 JSON
     */
    private String appointmentTypeJson;

    /**
     * 备注
     */
    private String commentText;
}
