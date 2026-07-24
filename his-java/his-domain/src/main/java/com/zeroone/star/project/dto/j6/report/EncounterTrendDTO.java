package com.zeroone.star.project.dto.j6.report;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * <p>
 * 描述：周、月接诊量门诊趋势数据传输对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
@Data
@Schema(description = "周、月接诊量门诊趋势数据传输对象")
public class EncounterTrendDTO {
    @Schema(description = "日期", example = "2026-07-19")
    private String date;

    @Schema(description = "接诊人数", example = "35")
    private Integer value;
}
