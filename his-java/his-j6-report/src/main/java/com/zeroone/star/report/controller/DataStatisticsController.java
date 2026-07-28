package com.zeroone.star.report.controller;

import com.zeroone.star.project.j6.report.DataStatisticsApis;
import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;

import com.zeroone.star.report.service.IReportService;
import jakarta.annotation.Resource;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;


@RestController
@RequestMapping("/data-statistics")
@Tag(name = "数据统计")
public class DataStatisticsController implements DataStatisticsApis {

    @Resource
    private IReportService reportService;

    @Operation(summary = "获取月访问量")
    @GetMapping("monthly-access")
    @Override
    public JsonVO<List<MonthlyAccessVO>> queryMonthlyAccess(MonthlyAccessQuery query) {
        List<MonthlyAccessVO> list = reportService.getMonthlyAccess(query);
        return JsonVO.success(list);
    }


    @Operation(summary = "物资入库与出库金额对比")
    @GetMapping("supply-amount-compare")
    @Override
    public JsonVO<List<SupplyAmountCompareVO>> querySupplyAmountCompare(SupplyAmountCompareQuery query) {
        List<SupplyAmountCompareVO> list = reportService.getSupplyAmountCompare(query);
        return JsonVO.success(list);
    }


    @Operation(summary = "库存物资有效期预警分布")
    @GetMapping("expiration-warning")
    @Override
    public JsonVO<List<ExpirationWarningVO>> queryExpirationWarning(ExpirationWarningQuery query) {
        List<ExpirationWarningVO> list = reportService.getExpirationWarning(query);
        return JsonVO.success(list);
    }


    @Operation(summary = "周接诊量门诊趋势")
    @GetMapping("/week-trend")
    @Override
    public JsonVO<EncounterWeekTrendVO> queryWeekTrend(EncounterWeekTrendQuery query) {
        return null;
    }

    @Operation(summary = "月接诊量门诊趋势")
    @GetMapping("/month-trend")
    @Override
    public JsonVO<EncounterMonthTrendVO> queryMonthTrend(EncounterMonthTrendQuery query) {
        return null;
    }

    @Operation(summary = "患者疾病类型分布")
    @GetMapping("/disease-statistic")
    @Override
    public JsonVO<DiseaseDistributionVO> queryDiseaseStatistic() {
        return null;
    }



    @Operation(summary = "获取流量趋势")
    @GetMapping("traffic-trend")
    @Override
    public JsonVO<List<TrafficTrendVO>> queryTrafficTrend(TrafficTrendQuery query) {
        // 创建 Mock 模拟测试数据，以保证接口文档能呈现出参示例
        List<TrafficTrendVO> list = new ArrayList<>();
        list.add(new TrafficTrendVO("2026-07-01", 320L));
        list.add(new TrafficTrendVO("2026-07-02", 450L));
        list.add(new TrafficTrendVO("2026-07-03", 380L));
        list.add(new TrafficTrendVO("2026-07-04", 510L));
        list.add(new TrafficTrendVO("2026-07-05", 600L));
        list.add(new TrafficTrendVO("2026-07-06", 280L));
        list.add(new TrafficTrendVO("2026-07-07", 430L));
        list.add(new TrafficTrendVO("2026-07-08", 520L));
        list.add(new TrafficTrendVO("2026-07-09", 480L));
        list.add(new TrafficTrendVO("2026-07-10", 550L));
        list.add(new TrafficTrendVO("2026-07-11", 390L));
        list.add(new TrafficTrendVO("2026-07-12", 460L));
        list.add(new TrafficTrendVO("2026-07-13", 510L));
        list.add(new TrafficTrendVO("2026-07-14", 580L));
        return JsonVO.success(list);
    }
}



