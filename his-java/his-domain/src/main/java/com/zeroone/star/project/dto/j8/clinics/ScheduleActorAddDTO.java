package com.zeroone.star.project.dto.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

@Data
@Schema(description = "日程参与者新增参数")
public class ScheduleActorAddDTO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "参与者类型编码", requiredMode = Schema.RequiredMode.REQUIRED)
    private String actorType;

    @Schema(description = "参与者ID", requiredMode = Schema.RequiredMode.REQUIRED)
    private String actorId;

    @Schema(description = "排序号")
    private Integer sortNo;
}
