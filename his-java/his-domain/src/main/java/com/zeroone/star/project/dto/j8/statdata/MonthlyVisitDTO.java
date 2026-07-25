package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;

@Data
@Schema(description = "月接诊量趋势查询参数")
public class MonthlyVisitDTO {

    @Schema(description = "年月", example = "2026-07",
            requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "年月不能为空")
    private String yearMonth;

    @Schema(description = "科室ID（可选）", example = "dept001")
    private String deptId;
}