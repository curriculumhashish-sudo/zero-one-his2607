package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "预约队列")
public class PatientQueueVO {

    @Schema(description = "主键ID", example = "1")
    private String id;

    @Schema(description = "预约分类", example = "OUTPATIENT_BOOKING")
    private String appointment_kind;

    @Schema(description = "业务号", example = "BK20240115001")
    private String bus_no;

    @Schema(description = "患者标识", example = "1")
    private String patient_id;

    @Schema(description = "患者姓名", example = "张三")
    private String patient_name;

    @Schema(description = "患者性别", example = "MALE")
    private String patient_gender;

    @Schema(description = "患者年龄", example = "30")
    private String patient_age;

    @Schema(description = "就诊标识", example = "1")
    private String encounter_id;

    @Schema(description = "机构标识", example = "1")
    private String organization_id;

    @Schema(description = "去向诊室/地点标识", example = "1")
    private String destination_location_id;

    @Schema(description = "去向诊室名称", example = "内科诊室")
    private String destination_location_name;

    @Schema(description = "分诊等级", example = "LEVEL1")
    private String triage_level;

    @Schema(description = "分诊等级名称", example = "一级")
    private String triage_level_name;

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

    @Schema(description = "服务类型名称", example = "普通门诊")
    private String service_type_name;

    @Schema(description = "预约类型编码", example = "NORMAL")
    private String appointment_type_code;

    @Schema(description = "预约类型名称", example = "普通预约")
    private String appointment_type_name;

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

    @Schema(description = "分诊状态", example = "WAITING")
    private String triage_status;

    @Schema(description = "分诊状态名称", example = "候诊中")
    private String triage_status_name;

    @Schema(description = "创建时间", example = "2024-01-15 08:00:00")
    private String created_at;
}