package com.zeroone.star.project.vo.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Schema(description = "月访问量数据点")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class MonthlyAccessVO {
    @Schema(description = "月份 (1-12)", example = "1")
    private Integer month;

    @Schema(description = "访问次数(登录成功量)", example = "1500")
    private Long count;
}
