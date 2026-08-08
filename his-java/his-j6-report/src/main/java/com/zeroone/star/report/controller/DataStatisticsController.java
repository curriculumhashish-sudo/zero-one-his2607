package com.zeroone.star.report.controller;

import com.zeroone.star.project.j6.report.DataStatisticsApis;
import com.zeroone.star.project.query.j6.report.DiseaseStatisticQuery;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;
import com.zeroone.star.report.service.IDataStatisticsService;
import com.zeroone.star.report.service.IReportService;
import com.zeroone.star.report.service.ITrafficTrendService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/data-statistics")
@RequiredArgsConstructor
@Tag(name = "数据统计", description = "对应 J8 statdata 接口组")
public class DataStatisticsController implements DataStatisticsApis {

    @Resource
    private ITrafficTrendService trafficTrendService;

    @Resource
    private IReportService reportService;

    private final IDataStatisticsService dataStatisticsService;

    @Operation(summary = "获取月访问量", description = "对应 J8 getMonthlyVisitTrend")
    @GetMapping("monthly-access")
    @Override
    public JsonVO<MonthlyAccessVO> queryMonthlyAccess(MonthlyAccessQuery query) {
        return JsonVO.success(reportService.getMonthlyAccess(query));
    }

    @Operation(summary = "物资出入库金额对比", description = "对应 J8 getStockInOutChart")
    @GetMapping("supply-amount-compare")
    @Override
    public JsonVO<SupplyAmountCompareVO> querySupplyAmountCompare(SupplyAmountCompareQuery query) {
        return JsonVO.success(reportService.getSupplyAmountCompare(query));
    }

    @Operation(summary = "库存物资有效期预警分布", description = "对应 J8 getExpireWarningPie")
    @GetMapping("expiration-warning")
    @Override
    public JsonVO<ExpirationWarningVO> queryExpirationWarning(ExpirationWarningQuery query) {
        return JsonVO.success(reportService.getExpirationWarning(query));
    }

    @Operation(summary = "周接诊量门诊趋势", description = "对应 J8 getVisitTrendWeekly")
    @GetMapping("/week-trend")
    @Override
    public JsonVO<EncounterWeekTrendVO> queryWeekTrend(EncounterWeekTrendQuery query) {
        return JsonVO.success(reportService.getWeekTrend(query));
    }

    @Operation(summary = "月接诊量门诊趋势", description = "对应 J8 getMonthlyVisitTrend")
    @GetMapping("/month-trend")
    @Override
    public JsonVO<EncounterMonthTrendVO> queryMonthTrend(EncounterMonthTrendQuery query) {
        return JsonVO.success(reportService.getMonthTrend(query));
    }

    @Operation(summary = "患者疾病类型分布", description = "对应 J8 getDiseaseDistribution")
    @GetMapping("/disease-statistic")
    @Override
    public JsonVO<DiseaseDistributionVO> queryDiseaseStatistic(DiseaseStatisticQuery query) {
        return JsonVO.success(reportService.getDiseaseDistribution(query));
    }

    @Operation(summary = "获取流量趋势", description = "对应 J8 getHourTrend")
    @GetMapping("traffic-trend")
    @Override
    public JsonVO<TrafficTrendVO> queryTrafficTrend(TrafficTrendQuery query) {
        return JsonVO.success(trafficTrendService.queryTrafficTrend(query));
    }
}
