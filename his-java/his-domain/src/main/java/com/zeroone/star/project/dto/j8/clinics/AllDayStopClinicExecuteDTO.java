package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "整日停诊执行信息")
public class AllDayStopClinicExecuteDTO {
    @Schema(description = "排班日程ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String scheduleId;

    @Schema(description = "机构ID", example = "ORG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "原因说明", example = "医师临时有事")
    private String reason;

    @Schema(description = "是否发送短信通知", example = "true")
    private Boolean sendNotification;
}