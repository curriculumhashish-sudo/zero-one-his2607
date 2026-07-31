package com.zeroone.star.project.vo.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;

/**
 * 患者队列列表返回VO
 */
@Data
@Schema(description = "患者队列返回结果")
public class PatientQueueVO {



    private String keyword;        // 姓名 / 排队号

    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "预约分类", example = "OUTPATIENT_BOOKING")
    private String appointment_kind;

    @Schema(description = "业务号", example = "BK20240115001")
    private String bus_no;

    @Schema(description = "患者标识", example = "1")
    private String patient_id;

    @Schema(description = "就诊标识", example = "1")
    private String encounter_id;

    @Schema(description = "机构标识", example = "1")
    private String organization_id;

    @Schema(description = "去向诊室/地点标识", example = "1")
    private String destination_location_id;

    @Schema(description = "分诊等级", example = "LEVEL1")
    private String triage_level;

    @Schema(description = "分诊优先级编码", example = "P1")
    private String triage_priority_code;

    @Schema(description = "分诊时间", example = "2024-01-15 08:00:00")
    private String triage_time;

    @Schema(description = "候诊显示顺序", example = "1")
    private Integer display_order;

    @Schema(description = "号源标识", example = "1")
    private String slot_id;

    @Schema(description = "服务类型编码", example = "GENERAL")
    private String service_type_code;

    @Schema(description = "预约类型编码", example = "NORMAL")
    private String appointment_type_code;

    @Schema(description = "计划开始时间", example = "2024-01-15 09:00:00")
    private String start_instant;

    @Schema(description = "计划结束时间", example = "2024-01-15 10:00:00")
    private String end_instant;

    @Schema(description = "取消/终态原因", example = "患者取消")
    private String cancel_reason;

    @Schema(description = "取消/终态原因编码", example = "PATIENT_CANCEL")
    private String cancellation_reason_code;

    @Schema(description = "取消/过号时间", example = "2024-01-15 08:30:00")
    private String cancellation_at;

    @Schema(description = "最近叫号时间", example = "2024-01-15 09:00:00")
    private String called_at;

    @Schema(description = "最近叫号医师标识", example = "1")
    private String last_practitioner_id;

    @Schema(description = "渠道编码", example = "KIOSK")
    private String origin_channel;

    @Schema(description = "预约日期", example = "2024-01-15")
    private String booking_date;

    @Schema(description = "创建时间", example = "2024-01-15 08:00:00")
    private String created_at;
}