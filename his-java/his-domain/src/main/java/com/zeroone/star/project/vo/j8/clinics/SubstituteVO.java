package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "替诊响应信息")
public class SubstituteVO {
    @Schema(description = "排班日程ID")
    private String scheduleId;

    @Schema(description = "原医师ID")
    private String originalPractitionerId;

    @Schema(description = "替诊医师ID")
    private String substitutePractitionerId;

    @Schema(description = "替诊原因")
    private String substituteReason;

    @Schema(description = "是否已清除")
    private Boolean cleared;
}