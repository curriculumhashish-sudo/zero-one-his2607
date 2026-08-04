package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "停诊处置项目")
public class StopClinicItemVO {
    @Schema(description = "预约ID")
    private String appointmentId;

    @Schema(description = "就诊流水号")
    private String busNo;

    @Schema(description = "患者ID")
    private String patientId;

    @Schema(description = "患者姓名")
    private String patientName;

    @Schema(description = "源号源ID")
    private String sourceSlotId;

    @Schema(description = "源开始时间")
    private String sourceStartTime;

    @Schema(description = "源结束时间")
    private String sourceEndTime;

    @Schema(description = "目标号源ID")
    private String targetSlotId;

    @Schema(description = "目标开始时间")
    private String targetStartTime;

    @Schema(description = "目标结束时间")
    private String targetEndTime;

    @Schema(description = "处置状态")
    private String dispositionStatus;

    @Schema(description = "处置消息")
    private String dispositionMessage;
}