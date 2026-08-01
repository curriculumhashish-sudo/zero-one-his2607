package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.dto.j8.statdata.BaseStatQueryDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.DistributionVO;
import com.zeroone.star.project.vo.j8.statdata.TrendChartVO;
import com.zeroone.star.statdata.service.OutpatientService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/outpatient")
@Tag(name = "报表中心-数据统计-门诊数据")
public class OutpatientController {

    @Autowired
    private OutpatientService outpaientService;

    @GetMapping("/visit-trend/weekly")
    @Operation(summary = "获取周接诊量门诊趋势")
    public JsonVO<TrendChartVO> getVisitTrendWeekly(@Valid @ModelAttribute BaseStatQueryDTO dto) {
        return JsonVO.success(outpaientService.getWeeklyOutpatientData(dto));
    }

    @GetMapping("/visit-trend/monthly")
    @Operation(summary = "获取月接诊量门诊趋势")
    public JsonVO<TrendChartVO> getMonthlyVisitTrend(@Valid @ModelAttribute BaseStatQueryDTO dto) {
        return JsonVO.success(outpaientService.getMonthlyOutpatientData(dto));
    }

    @GetMapping("/disease-distribution")
    @Operation(summary = "获取患者疾病类型分布")
    public JsonVO<DistributionVO> getDiseaseDistribution(@Valid @ModelAttribute BaseStatQueryDTO dto) {
        return JsonVO.success(outpaientService.getDiseaseDistribution(dto));
    }
}
