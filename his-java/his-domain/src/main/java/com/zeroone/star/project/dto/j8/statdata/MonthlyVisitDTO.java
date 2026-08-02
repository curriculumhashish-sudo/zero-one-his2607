package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.time.LocalDate;

@Data
@Schema(description = "月接诊量趋势查询参数")
public class MonthlyVisitDTO {

    @Schema(description = "年月", example = "2026-07",
            requiredMode = Schema.RequiredMode.REQUIRED)
    @NotNull(message = "年月不能为空")
    private LocalDate date;

}