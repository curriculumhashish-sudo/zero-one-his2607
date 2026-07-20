package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@Schema(description = "日程新增数据传输对象")
public class ScheduleAddDTO {
    @Schema(description = "名称", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String name;
    @Schema(description = "号别类型:医师号 0,科室号 1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer type;
    @Schema(description = "出诊医师ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String doctorId;
    @Schema(description = "号源类型:便民,普通,专家", requiredMode = Schema.RequiredMode.REQUIRED)
    private String slotType;
    @Schema(description = "放号时段:工作日,正常时段,不限号", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private String slotTime;
    @Schema(description = "出诊日期", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDateTime date;
}
