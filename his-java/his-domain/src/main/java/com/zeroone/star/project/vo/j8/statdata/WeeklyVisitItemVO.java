package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "周接诊量折线图数据点")
public class WeeklyVisitItemVO {

    @Schema(description = "星期名称", example = "周一",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String dayName;           // 周一、周二、...、周日

    @Schema(description = "排序索引", example = "1",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer dayIndex;         // 1~7，前端按此排序

    @Schema(description = "接诊量", example = "856",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer count;
}