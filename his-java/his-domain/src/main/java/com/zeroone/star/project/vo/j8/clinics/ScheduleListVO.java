package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDate;
import java.util.List;

@Data
@Schema(description = "日程列表响应")
public class ScheduleListVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "日程日期列表")
    private List<ScheduleDayVO> days;

    @Schema(description = "年月")
    private String yearMonth;

    @Schema(description = "月起始日期")
    private LocalDate monthStart;

    @Schema(description = "月结束日期")
    private LocalDate monthEnd;

    @Schema(description = "网格起始日期")
    private LocalDate gridStart;

    @Schema(description = "网格结束日期")
    private LocalDate gridEnd;

    @Schema(description = "汇总信息")
    private ScheduleSummaryVO summary;
}
