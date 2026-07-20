package com.zeroone.star.project.vo.j5.ons;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "患者队列")
public class PatientQueueVO {

    @Schema(description = "排队ID", example = "1")
    private String id;

    @Schema(description = "患者ID", example = "1")
    private String patient_id;

    @Schema(description = "患者姓名", example = "张三")
    private String patient_name;

    @Schema(description = "患者性别", example = "男")
    private String patient_gender;

    @Schema(description = "患者年龄", example = "30")
    private Integer patient_age;

    @Schema(description = "患者卡号", example = "P000001")
    private String patient_card_no;

    @Schema(description = "科室ID", example = "1")
    private String department_id;

    @Schema(description = "科室名称", example = "内科")
    private String department_name;

    @Schema(description = "医生ID", example = "1")
    private String doctor_id;

    @Schema(description = "医生姓名", example = "王医生")
    private String doctor_name;

    @Schema(description = "就诊号", example = "V20240115001")
    private String visit_no;

    @Schema(description = "就诊序号", example = "1")
    private Integer visit_order;

    @Schema(description = "就诊状态（0-待就诊，1-就诊中，2-已完成）", example = "0")
    private Integer status;

    @Schema(description = "就诊状态名称", example = "待就诊")
    private String status_name;

    @Schema(description = "挂号时间", example = "2024-01-15 08:00:00")
    private String register_time;

    @Schema(description = "预计就诊时间", example = "2024-01-15 09:00:00")
    private String expected_visit_time;
}