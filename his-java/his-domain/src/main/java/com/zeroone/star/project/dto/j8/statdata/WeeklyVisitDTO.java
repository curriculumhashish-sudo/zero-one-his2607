package com.zeroone.star.project.dto.j8.statdata;

import io.swagger.v3.oas.annotations.media.Schema;
import java.time.LocalDate;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;
import org.apache.fesod.sheet.annotation.format.DateTimeFormat;

@Data
@Schema(description = "周接诊量趋势查询参数")
public class WeeklyVisitDTO {

    @Schema(description = "所选周的周中日期",
            example = "2026-07-20",
            requiredMode = Schema.RequiredMode.REQUIRED)
    @NotNull(message = "周中日期不能为空")
    @DateTimeFormat(value = "yyyy-MM-dd")
    private LocalDate weekCenter;
}