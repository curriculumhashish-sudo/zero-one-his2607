package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@Schema(description = "日程管理对象")
public class ScheduleDTO extends ScheduleAddDTO {
    @Schema(description = "日程ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
    @Schema(description = "科室ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String clinicId;
    @Schema(description = "医师", requiredMode = Schema.RequiredMode.REQUIRED)
    private String doctor;
    @Schema(description = "号源类型:便民,普通,专家", requiredMode = Schema.RequiredMode.REQUIRED)
    private String slotType;
    @Schema(description = "状态:草稿 0,已发布 1")
    private Integer scheduleType;
    @Schema(description = "开始日期")
    private LocalDateTime startDate;
    @Schema(description = "结束日期")
    private LocalDateTime endDate;
}