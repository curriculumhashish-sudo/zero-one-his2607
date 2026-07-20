package com.zeroone.star.project.vo.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "日排班下拉选项")
public class DailyScheduleOptionVO {
    @Schema(description = "排班模板ID", example = "tpl-001")
    private String id;
    @Schema(description = "排班模板名称", example = "门诊医生")
    private String templateName;
}
