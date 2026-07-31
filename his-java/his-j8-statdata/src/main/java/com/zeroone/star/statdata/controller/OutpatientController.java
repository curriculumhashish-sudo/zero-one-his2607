package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.dto.j8.statdata.DiseaseDistributionDTO;
import com.zeroone.star.project.dto.j8.statdata.MonthTrendDTO;
import com.zeroone.star.project.dto.j8.statdata.MonthlyVisitDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.DiseaseDistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.MonthlyVisitItemVO;
import com.zeroone.star.project.vo.j8.statdata.WeeklyVisitItemVO;
import com.zeroone.star.statdata.service.OutpatientService;
import io.swagger.v3.oas.annotations.Operation;
import jakarta.validation.Valid;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import com.zeroone.star.project.dto.j8.statdata.WeeklyVisitDTO;

import java.time.LocalDate;
import java.util.List;

@RestController
@RequestMapping("/outpatient")
public class OutpatientController {

    @Autowired
    private OutpatientService outpaientService;

    @GetMapping("/visit-trend/weekly")
    @Operation(summary = "获取周接诊量门诊趋势")
    public JsonVO<List<WeeklyVisitItemVO>>  getVisitTrendWeekly(@Valid @ModelAttribute WeeklyVisitDTO DTO) {
        LocalDate date = DTO.getWeekCenter();
        if (date.isAfter(LocalDate.now())) {
            return JsonVO.create(null, ResultStatus.PARAMS_INVALID.getCode(), "不能查询未来日期");
        }
        List<WeeklyVisitItemVO> weeklyOutpatientData = outpaientService.getWeeklyOutpatientData(date);
        return JsonVO.success(weeklyOutpatientData);
    }

    @GetMapping("/visit-trend/monthly")
    @Operation(summary = "获取月接诊量门诊趋势")
    public JsonVO<List<MonthlyVisitItemVO>> getMonthlyVisitTrend(@Valid @ModelAttribute MonthlyVisitDTO DTO) {
        LocalDate date = DTO.getDate();
        if (date.isAfter(LocalDate.now())) {
            return JsonVO.create(null, ResultStatus.PARAMS_INVALID.getCode(), "不能查询未来日期");
        }
        List<MonthlyVisitItemVO> monthlyOutpatientData = outpaientService.getMonthlyOutpatientData(date);
        return JsonVO.success(monthlyOutpatientData);
    }


    @GetMapping("/disease-distribution")
    @Operation(summary = "获取患者疾病类型分布")
    public JsonVO<List<DiseaseDistributionItemVO>> getDiseaseDistribution(){
        List<DiseaseDistributionItemVO> result = outpaientService.getDiseaseDistribution();
        return JsonVO.success(result);
    }
}
