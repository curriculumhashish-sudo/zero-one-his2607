package com.zeroone.star.project.vo.j8.setroster;


import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "排班日程视图")
public class ScheduleVO {
    @Schema(description = "排班日程ID", example = "1")
    private Long id;

    @Schema(description = "医师ID", example = "100")
    private Long practitionerId;

    @Schema(description = "医师姓名", example = "李医生")
    private String practitionerName;

    @Schema(description = "替诊医师ID", example = "101")
    private Long substitutePractitionerId;

    @Schema(description = "替诊医师姓名", example = "王医生")
    private String substitutePractitionerName;

    @Schema(description = "替诊原因", example = "医师请假")
    private String substituteReason;

    @Schema(description = "状态：active-已发布，inactive-已停用", example = "active")
    private String statusEnum;

    @Schema(description = "排班日期", example = "2026-07-20")
    private String scheduleDate;

    @Schema(description = "开始时间", example = "08:00")
    private String startTime;

    @Schema(description = "结束时间", example = "17:00")
    private String endTime;

    @Schema(description = "科室ID", example = "10")
    private Long deptId;

    @Schema(description = "科室名称", example = "内科")
    private String deptName;
}