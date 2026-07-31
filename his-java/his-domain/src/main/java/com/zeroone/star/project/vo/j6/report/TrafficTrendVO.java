package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Schema(description = "流量趋势数据点")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class TrafficTrendVO {

    @Schema(description = "日期（根据时间粒度改变月/日/周）", example = "2026-07-01")
    private String date;

    @Schema(description = "访问量", example = "1500")
    private Long count;
}
