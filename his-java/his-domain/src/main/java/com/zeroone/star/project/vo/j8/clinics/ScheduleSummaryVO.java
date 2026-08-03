package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

@Data
@Schema(description = "日程列表汇总信息")
public class ScheduleSummaryVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "已发布天数")
    private Integer publishedDayCount;

    @Schema(description = "草稿天数")
    private Integer draftDayCount;

    @Schema(description = "未排班天数")
    private Integer unscheduledDayCount;

    @Schema(description = "失效天数")
    private Integer inactiveDayCount;

    @Schema(description = "已预约数")
    private Integer bookedAppointmentCount;

    @Schema(description = "日程总数")
    private Integer totalScheduleCount;
}
