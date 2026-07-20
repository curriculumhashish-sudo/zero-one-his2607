package com.zeroone.star.project.query.j8.setroster;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "排班日程查询对象")
public class ScheduleQuery extends PageQuery {
    @Schema(description = "医师ID", example = "100")
    private Long practitionerId;

    @Schema(description = "科室ID", example = "10")
    private Long deptId;

    @Schema(description = "排班日期", example = "2026-07-20")
    private String scheduleDate;

    @Schema(description = "状态：active-已发布，inactive-已停用", example = "active")
    private String statusEnum;
}