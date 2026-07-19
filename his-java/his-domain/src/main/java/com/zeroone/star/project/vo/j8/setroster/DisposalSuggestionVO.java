package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "处置建议视图")
public class DisposalSuggestionVO {
    @Schema(description = "预约ID", example = "1001")
    private Long appointmentId;

    @Schema(description = "建议处置方式：TRANSFER-转交，RESCHEDULE-改期，CANCEL-取消，MANUAL-人工", example = "TRANSFER")
    private String suggestedDisposalType;

    @Schema(description = "建议说明", example = "建议转交至同时间段其他医师")
    private String suggestionReason;

    @Schema(description = "目标号源ID（转交时）", example = "2002")
    private Long targetSlotId;

    @Schema(description = "目标号源时间（转交时）", example = "09:00-10:00")
    private String targetSlotTime;
}