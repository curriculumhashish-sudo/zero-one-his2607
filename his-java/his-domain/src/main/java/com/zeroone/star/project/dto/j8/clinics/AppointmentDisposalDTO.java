package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 预约处置信息 DTO
 */
@Data
@Schema(description = "预约处置信息")
public class AppointmentDisposalDTO {

    @Schema(description = "预约ID", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long appointmentId;

    @Schema(description = "处置方式：TRANSFER-转交，CANCEL-取消，RESCHEDULE-改期，MANUAL-人工",
            example = "TRANSFER", requiredMode = Schema.RequiredMode.REQUIRED)
    private String disposalType;

    @Schema(description = "目标号源ID（转交时使用）", example = "2001")
    private Long targetSlotId;

    @Schema(description = "取消原因编码", example = "PATIENT_REQUEST")
    private String cancelReasonCode;

    @Schema(description = "取消原因说明", example = "患者主动要求取消")
    private String cancelReason;
}