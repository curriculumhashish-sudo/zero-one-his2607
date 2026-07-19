package com.zeroone.star.project.query.j6.report;
// 周接诊量门诊趋势查询：日期范围(start/end)

import io.swagger.v3.oas.annotations.media.Schema;
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
    private LocalDate startDate;  // 可选，默认最近7天
    private LocalDate endDate;    // 可选，默认今天
}
