package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "日程新增数据传输对象")
public class ScheduleAddDTO {

    @Schema(description = "机构标识", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "日程名称")
    private String name;

    @Schema(description = "规划窗口起始时间", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime planningHorizonStart;

    @Schema(description = "规划窗口结束时间", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime planningHorizonEnd;

    @Schema(description = "号源模板ID")
    private String slotProvisionTemplateId;

    @Schema(description = "参与者列表")
    private List<ScheduleActorAddDTO> actors;
}
