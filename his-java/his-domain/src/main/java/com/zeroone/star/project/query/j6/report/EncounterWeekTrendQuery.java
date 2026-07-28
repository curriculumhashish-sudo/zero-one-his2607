package com.zeroone.star.project.query.j6.report;
// 周接诊量门诊趋势查询：日期范围(start/end)

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.AssertTrue;
import lombok.Data;

import java.time.LocalDate;

/**
 * <p>
 * 描述：周接诊量门诊趋势查询对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
@Data
@Schema(description = "周接诊量门诊趋势查询对象")
public class EncounterWeekTrendQuery {
    @Schema(description = "开始日期", example = "2026-07-01")
    private LocalDate startDate;  // 可选，默认最近7天

    @Schema(description = "结束日期", example = "2026-07-07")
    private LocalDate endDate;    // 可选，默认今天

    @AssertTrue(message = "开始日期不能晚于结束日期")
    public boolean isDateRangeValid() {
        if (startDate != null && endDate != null) {
            return !startDate.isAfter(endDate);
        }
        return true;
    }
}
