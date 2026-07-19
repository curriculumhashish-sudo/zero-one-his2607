package com.zeroone.star.report.controller;

import com.zeroone.star.project.j6.report.ReportApis;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：报表管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
@RestController
@Tag(name = "报表管理")
public class ReportController implements ReportApis {
    @Operation(summary = "查询周接诊量门诊趋势")
    @GetMapping("/week-trend")
    @Override
    public JsonVO<EncounterWeekTrendVO> queryWeekTrend(EncounterWeekTrendQuery query) {
        return null;
    }

    @Operation(summary = "查询月接诊量门诊趋势")
    @GetMapping("/month-trend")
    @Override
    public JsonVO<EncounterMonthTrendVO> queryMonthTrend(EncounterMonthTrendQuery query) {
        return null;
    }

    @Operation(summary = "查询患者疾病类型分布")
    @GetMapping("/disease-statistic")
    @Override
    public JsonVO<DiseaseDistributionVO> queryDiseaseStatistic() {
        return null;
    }
}
