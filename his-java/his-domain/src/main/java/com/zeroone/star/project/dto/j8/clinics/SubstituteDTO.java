package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "替诊信息")
public class SubstituteDTO {
    @Schema(description = "排班日程ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long scheduleId;

    @Schema(description = "替诊医师ID", example = "100", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long substitutePractitionerId;

    @Schema(description = "替诊原因", example = "医师请假")
    private String substituteReason;
}