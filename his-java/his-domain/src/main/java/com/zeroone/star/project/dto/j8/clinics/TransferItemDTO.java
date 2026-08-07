package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "转交项目信息")
public class TransferItemDTO {
    @Schema(description = "预约ID", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String appointmentId;

    @Schema(description = "目标号源ID", example = "2001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String targetSlotId;
}