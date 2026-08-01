package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDateTime;

/**
 * <p>
 * 日程参与者（医师/诊室/设备等）
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("wkf_schedule_actor")
public class WkfScheduleActor implements Serializable {

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
    private Integer version;

    /**
     * 所属日程标识
     */
    private String scheduleId;

    /**
     * 参与者类型编码
     */
    private Integer actorType;

    /**
     * 参与者主键
     */
    private String actorId;

    /**
     * 排序号
     */
    private Integer sortNo;
}
