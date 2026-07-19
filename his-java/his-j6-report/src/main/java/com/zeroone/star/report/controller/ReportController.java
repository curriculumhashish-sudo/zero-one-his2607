package com.zeroone.star.report.controller;

import com.zeroone.star.project.j6.report.ReportApis;
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

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;


@RestController
@RequestMapping("/report")
@Tag(name = "report-center", description = "报表中心")
public class ReportController implements ReportApis {

    @Operation(summary = "获取月访问量")
    @GetMapping("monthly-access")
    @Override
    public JsonVO<List<MonthlyAccessVO>> queryMonthlyAccess(MonthlyAccessQuery query) {
        // 创建 Mock 模拟测试数据，以保证接口文档能呈现响应结果示例值
        List<MonthlyAccessVO> list = new ArrayList<>();
        list.add(new MonthlyAccessVO(1, 1500L));
        list.add(new MonthlyAccessVO(2, 1800L));
        list.add(new MonthlyAccessVO(3, 1200L));
        list.add(new MonthlyAccessVO(4, 2100L));
        list.add(new MonthlyAccessVO(5, 1700L));
        list.add(new MonthlyAccessVO(6, 2500L));
        list.add(new MonthlyAccessVO(7, 3000L));
        list.add(new MonthlyAccessVO(8, 2800L));
        list.add(new MonthlyAccessVO(9, 2200L));
        list.add(new MonthlyAccessVO(10, 1900L));
        list.add(new MonthlyAccessVO(11, 2400L));
        list.add(new MonthlyAccessVO(12, 2600L));
        return JsonVO.success(list);
    }

    @Operation(summary = "物资入库与出库金额对比")
    @GetMapping("supply-amount-compare")
    @Override
    public JsonVO<List<SupplyAmountCompareVO>> querySupplyAmountCompare(SupplyAmountCompareQuery query) {
        // 创建 Mock 模拟测试数据，以保证接口文档能呈现出参示例
        List<SupplyAmountCompareVO> list = new ArrayList<>();
        // 2026-01
        list.add(new SupplyAmountCompareVO("INBOUND", new BigDecimal("120000.00"), "2026-01"));
        list.add(new SupplyAmountCompareVO("OUTBOUND", new BigDecimal("95000.00"), "2026-01"));
        // 2026-02
        list.add(new SupplyAmountCompareVO("INBOUND", new BigDecimal("150000.00"), "2026-02"));
        list.add(new SupplyAmountCompareVO("OUTBOUND", new BigDecimal("110000.00"), "2026-02"));
        // 2026-03
        list.add(new SupplyAmountCompareVO("INBOUND", new BigDecimal("80000.00"), "2026-03"));
        list.add(new SupplyAmountCompareVO("OUTBOUND", new BigDecimal("85000.00"), "2026-03"));
        // 2026-04
        list.add(new SupplyAmountCompareVO("INBOUND", new BigDecimal("200000.00"), "2026-04"));
        list.add(new SupplyAmountCompareVO("OUTBOUND", new BigDecimal("180000.00"), "2026-04"));
        // 2026-05
        list.add(new SupplyAmountCompareVO("INBOUND", new BigDecimal("170000.00"), "2026-05"));
        list.add(new SupplyAmountCompareVO("OUTBOUND", new BigDecimal("150000.00"), "2026-05"));
        // 2026-06
        list.add(new SupplyAmountCompareVO("INBOUND", new BigDecimal("250000.00"), "2026-06"));
        list.add(new SupplyAmountCompareVO("OUTBOUND", new BigDecimal("220000.00"), "2026-06"));
        return JsonVO.success(list);
    }

    @Operation(summary = "库存物资有效期预警分布")
    @GetMapping("expiration-warning")
    @Override
    public JsonVO<List<ExpirationWarningVO>> queryExpirationWarning(ExpirationWarningQuery query) {
        // 创建 Mock 模拟测试数据，以保证接口文档能呈现出参示例
        List<ExpirationWarningVO> list = new ArrayList<>();
        list.add(new ExpirationWarningVO("阿莫西林胶囊", "LOT20260601", "2026-08-15", 27, new BigDecimal("1200.00"), "西药房", "国药控股股份有限公司"));
        list.add(new ExpirationWarningVO("一次性注射器 (2ml)", "LOT20260120", "2026-09-01", 44, new BigDecimal("8500.00"), "物资总库", "江苏鱼跃医疗设备股份有限公司"));
        list.add(new ExpirationWarningVO("葡萄糖注射液 5%", "LOT20260610", "2026-09-20", 63, new BigDecimal("340.00"), "西药房", "华润医药集团有限公司"));
        list.add(new ExpirationWarningVO("医用防护口罩", "LOT20251215", "2026-10-15", 88, new BigDecimal("20000.00"), "医用耗材库", "3M中国有限公司"));
        return JsonVO.success(list);
    }
}



