package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.DiseaseDistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.MonthlyVisitItemVO;
import com.zeroone.star.project.vo.j8.statdata.WeeklyVisitItemVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.List;

@RestController
@RequestMapping("/statdata/outpatient")
public class OutpatientController {
    @GetMapping("/visit-trend/weekly")
    @Operation(summary = "获取周接诊量门诊趋势")
    public JsonVO<List<WeeklyVisitItemVO>>  getVisitTrendWeekly(WeeklyVisitItemVO weeklyVisitItemVO) {
        return JsonVO.success(new ArrayList<>());
    }

    @GetMapping("/visit-trend/monthly")
    @Operation(summary = "获取月接诊量门诊趋势")
    public JsonVO<List<MonthlyVisitItemVO>> getMonthlyVisitTrend(MonthlyVisitItemVO monthlyVisitItemVO) {
        return JsonVO.success(new ArrayList<>());
    }


    @GetMapping("/disease-distribution")
    @Operation(summary = "获取患者疾病类型分布")
    public JsonVO<List<DiseaseDistributionItemVO>> getDiseaseDistribution(DiseaseDistributionItemVO diseaseDistributionItemVO){
        return JsonVO.success(new ArrayList<>());
    }
}
