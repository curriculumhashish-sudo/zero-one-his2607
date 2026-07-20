package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "周接诊量趋势查询参数")
public class WeeklyVisitDTO {

    @Schema(description = "所选周的周一日期", example = "2026-07-20",
            requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "周一日期不能为空")
    private String monday;
}