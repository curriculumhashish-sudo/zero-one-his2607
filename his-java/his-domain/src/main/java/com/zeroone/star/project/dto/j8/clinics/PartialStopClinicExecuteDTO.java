package com.zeroone.star.project.dto.j8.clinics;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "部分停诊执行信息")
public class PartialStopClinicExecuteDTO {
    @Schema(description = "排班日程ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String scheduleId;

    @Schema(description = "机构ID", example = "ORG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "号源ID列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> slotIds;

    @Schema(description = "处置模式：TRANSFER-转交，CANCEL-取消，RESCHEDULE-改期", example = "TRANSFER", requiredMode = Schema.RequiredMode.REQUIRED)
    private String dispositionMode;

    @Schema(description = "目标医师ID", example = "DOC001")
    private String targetPractitionerId;

    @Schema(description = "转交项目列表")
    private List<TransferItemDTO> transferItems;

    @Schema(description = "原因说明", example = "医师临时有事")
    private String reason;

    @Schema(description = "是否延迟人工处理", example = "false")
    private Boolean deferManualHandling;

    @Schema(description = "是否发送短信通知", example = "true")
    private Boolean sendNotification;
}