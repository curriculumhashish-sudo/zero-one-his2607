package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.List;

@Data
@Schema(description = "日程日期信息")
public class ScheduleDayVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "日期")
    private LocalDate date;

    @Schema(description = "是否当月")
    private Boolean inMonth;

    @Schema(description = "生效日程数")
    private Integer activeCount;

    @Schema(description = "草稿日程数")
    private Integer draftCount;

    @Schema(description = "失效日程数")
    private Integer inactiveCount;

    @Schema(description = "日程列表")
    private List<ScheduleItemVO> schedules;
}
