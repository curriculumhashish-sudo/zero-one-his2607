package com.zeroone.star.project.vo.j6.report;

import com.zeroone.star.project.dto.j6.report.EncounterTrendDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

// 周接诊量趋势数据点：week, count
/**
 * <p>
 * 描述：周接诊量门诊趋势图表数据返回对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
@Data
@Schema(description = "周接诊量门诊趋势图表数据返回对象")
public class EncounterWeekTrendVO {
    @Schema(description = "周接诊量门诊趋势数据列表")
    private List<EncounterTrendDTO> data;
}
