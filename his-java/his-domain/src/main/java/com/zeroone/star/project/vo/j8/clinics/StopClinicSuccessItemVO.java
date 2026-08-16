package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "停诊成功项目")
public class StopClinicSuccessItemVO {
    @Schema(description = "预约ID")
    private String appointmentId;

    @Schema(description = "目标号源ID")
    private String targetSlotId;

    @Schema(description = "成功消息")
    private String message;
}