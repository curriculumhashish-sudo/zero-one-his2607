package com.zeroone.star.clinics.entity;

import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.io.Serializable;
import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * <p>
 * 预约与候诊队列（FHIR 预约 语义）
 * </p>
 *
 * @author aike
 * @since 2026-07-25
 */
@Getter
@Setter
@ToString
@TableName("wkf_appointment_queue")
public class WkfAppointmentQueue implements Serializable {

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
     * 预约分类（OUTPATIENT_BOOKING / CHECK_IN_ONLY 等）
     */
    private String appointmentKind;

    /**
     * 业务号
     */
    private String busNo;

    /**
     * 患者标识
     */
    private String patientId;

    /**
     * 就诊标识
     */
    private String encounterId;

    /**
     * 机构标识
     */
    private String organizationId;

    /**
     * 去向诊室/地点标识
     */
    private String destinationLocationId;

    /**
     * 分诊等级（文本）
     */
    private String triageLevel;

    /**
     * 分诊优先级编码
     */
    private String triagePriorityCode;

    /**
     * 分诊时间
     */
    private LocalDateTime triageTime;

    /**
     * 候诊显示顺序
     */
    private Integer displayOrder;

    /**
     * 号源标识（门诊预约）
     */
    private String slotId;

    /**
     * 服务类型编码
     */
    private String serviceTypeCode;

    /**
     * 预约类型编码
     */
    private String appointmentTypeCode;

    /**
     * 计划开始时间
     */
    private LocalDateTime startInstant;

    /**
     * 计划结束时间
     */
    private LocalDateTime endInstant;

    /**
     * 取消/终态原因（文本）
     */
    private String cancelReason;

    /**
     * 取消/终态原因编码
     */
    private String cancellationReasonCode;

    /**
     * 取消/过号时间
     */
    private LocalDateTime cancellationAt;

    /**
     * 最近叫号时间
     */
    private LocalDateTime calledAt;

    /**
     * 最近叫号医师标识
     */
    private String lastPractitionerId;

    /**
     * 渠道编码（KIOSK/MINIAPP/COUNTER 等）
     */
    private String originChannel;

    /**
     * 渠道会话标识
     */
    private String originChannelSessionId;

    /**
     * 来源设备标识
     */
    private String originSourceDeviceId;

    /**
     * 代操作操作者标识
     */
    private String originOperatorId;

    /**
     * 客户端版本号
     */
    private String originClientAppVersion;

    /**
     * 冗余：门诊防重（同 患者+service+booking_date）
     */
    private String healthcareServiceId;

    /**
     * 冗余：预约自然日（Asia/Shanghai）
     */
    private LocalDate bookingDate;
}
