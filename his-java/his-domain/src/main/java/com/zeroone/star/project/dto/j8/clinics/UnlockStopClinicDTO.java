package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

@Data
@Schema(description = "解锁停诊请求参数")
public class UnlockStopClinicDTO {
    @Schema(description = "排班日程ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String scheduleId;

    @Schema(description = "机构ID", example = "ORG001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "号源ID列表", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> slotIds;
}