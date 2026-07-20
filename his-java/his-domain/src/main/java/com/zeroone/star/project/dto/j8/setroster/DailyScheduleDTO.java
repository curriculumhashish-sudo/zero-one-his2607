package com.zeroone.star.project.dto.j8.setroster;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "日排班模板传输对象")
public class DailyScheduleDTO extends AddDailyScheduleDTO{
    @Schema(description = "排班ID（修改时必填，新增时忽略）", example = "s-001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;
}
