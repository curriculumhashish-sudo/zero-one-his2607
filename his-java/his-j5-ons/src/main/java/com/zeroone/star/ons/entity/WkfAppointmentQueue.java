package com.zeroone.star.ons.entity;

import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.time.LocalDateTime;

@Data
@EqualsAndHashCode(callSuper = true)
@TableName("wkf_appointment_queue")
public class WkfAppointmentQueue extends BaseEntity {

    @TableField("appointment_kind")
    private String appointmentKind;

    @TableField("bus_no")
    private String busNo;

    @TableField("patient_id")
    private String patientId;

    @TableField("encounter_id")
    private String encounterId;

    @TableField("organization_id")
    private String organizationId;

    @TableField("destination_location_id")
    private String destinationLocationId;

    @TableField("triage_level")
    private String triageLevel;

    @TableField("triage_priority_code")
    private String triagePriorityCode;

    @TableField("triage_time")
    private LocalDateTime triageTime;

    @TableField("display_order")
    private Integer displayOrder;

    @TableField("slot_id")
    private String slotId;

    @TableField("service_type_code")
    private String serviceTypeCode;

    @TableField("appointment_type_code")
    private String appointmentTypeCode;

    @TableField("start_instant")
    private LocalDateTime startInstant;

    @TableField("end_instant")
    private LocalDateTime endInstant;

    @TableField("cancel_reason")
    private String cancelReason;

    @TableField("cancellation_reason_code")
    private String cancellationReasonCode;

    @TableField("cancellation_at")
    private LocalDateTime cancellationAt;

    @TableField("called_at")
    private LocalDateTime calledAt;

    @TableField("last_practitioner_id")
    private String lastPractitionerId;

    @TableField("origin_channel")
    private String originChannel;

    @TableField("origin_channel_session_id")
    private String originChannelSessionId;

    @TableField("origin_source_device_id")
    private String originSourceDeviceId;

    @TableField("origin_operator_id")
    private String originOperatorId;

    @TableField("origin_client_app_version")
    private String originClientAppVersion;

    @TableField("healthcare_service_id")
    private String healthcareServiceId;

    @TableField("booking_date")
    private LocalDateTime bookingDate;
}