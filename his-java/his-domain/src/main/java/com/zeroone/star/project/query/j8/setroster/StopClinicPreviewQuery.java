package com.zeroone.star.project.query.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "停诊预览查询对象")
public class StopClinicPreviewQuery {
    @Schema(description = "排班日程ID", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long scheduleId;

    @Schema(description = "停诊类型：FULL_DAY-整日停诊，PARTIAL-部分停诊", example = "FULL_DAY", requiredMode = Schema.RequiredMode.REQUIRED)
    private String stopType;

    @Schema(description = "停诊开始时间（部分停诊时使用）", example = "09:00")
    private String stopStartTime;

    @Schema(description = "停诊结束时间（部分停诊时使用）", example = "12:00")
    private String stopEndTime;
}