package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.Serializable;

@Data
@NoArgsConstructor
@AllArgsConstructor
@Schema(description = "日程创建响应")
public class ScheduleCreateVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "日程ID")
    private String scheduleId;
}
