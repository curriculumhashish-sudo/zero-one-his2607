package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "执行停诊信息")
public class AllDayStopClinicExecuteDTO {
    @Schema(description = "排班日程ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long scheduleId;

    @Schema(description = "预约处置列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<AppointmentDisposalDTO> appointmentDisposals;

    @Schema(description = "是否发送通知", example = "true")
    private Boolean sendNotification;

    @Schema(description = "停诊原因", example = "医师请假")
    private String stopReason;
}