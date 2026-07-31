package com.zeroone.star.project.vo.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Schema(description = "月接诊量折线图数据点")
@AllArgsConstructor
@NoArgsConstructor
public class MonthlyVisitItemVO {

    @Schema(description = "区间标签", example = "1日",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String label;             // "1日", "3日", "5日", ..., "29日"

    @Schema(description = "日期范围", example = "1-2",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private String dateRange;         // "1-2", "3-4", ..., "29-31"

    @Schema(description = "接诊量", example = "856",
            requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer count;
}
