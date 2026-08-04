package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.time.LocalDate;

@Data
@Schema(description = "生成正式排班参数")
public class FormalRosterDTO {

    @Schema(description = "机构标识", requiredMode = Schema.RequiredMode.REQUIRED)
    private String organizationId;

    @Schema(description = "日排班模板ID")
    private String scheduleTemplateId;

    @Schema(description = "周排班模板ID")
    private String weekScheduleTemplateId;

    @Schema(description = "模板类型: day / week", requiredMode = Schema.RequiredMode.REQUIRED)
    private String templateKind;

    @Schema(description = "排班范围起始日期", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate rangeStart;

    @Schema(description = "排班范围结束日期", requiredMode = Schema.RequiredMode.REQUIRED)
    private LocalDate rangeEnd;

    @Schema(description = "是否自动发布")
    private Boolean autoPublish;

    @Schema(description = "是否自动放号")
    private Boolean autoProvision;
}
