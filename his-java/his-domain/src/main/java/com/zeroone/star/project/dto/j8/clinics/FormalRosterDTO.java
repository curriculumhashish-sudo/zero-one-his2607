package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDateTime;

@Data
@Schema(description = "生成正式排班数据传输对象")
public class FormalRosterDTO {
    @Schema(description = "排班类型:日排班 0,周排班 1",requiredMode =  Schema.RequiredMode.REQUIRED)
    private Integer rosterType;
    @Schema(description = "排班名称",requiredMode =  Schema.RequiredMode.REQUIRED)
    private String rosterName;
    @Schema(description = "排班开始时间",requiredMode =  Schema.RequiredMode.REQUIRED)
    private LocalDateTime startTime;
    @Schema(description = "排班结束时间",requiredMode =  Schema.RequiredMode.REQUIRED)
    private LocalDateTime endTime;
    @Schema(description = "是否自动发布:0-否,1-是 ",requiredMode =  Schema.RequiredMode.REQUIRED)
    private Integer isPublish;
    @Schema(description = "是否自动放号:0-否,1-是 ",requiredMode =  Schema.RequiredMode.REQUIRED)
    private Integer isSlotsRelease;


}
