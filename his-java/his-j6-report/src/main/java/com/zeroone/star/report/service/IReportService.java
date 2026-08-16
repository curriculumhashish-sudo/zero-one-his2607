package com.zeroone.star.report.service;

import com.zeroone.star.project.query.j6.report.DiseaseStatisticQuery;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;

/**
 * 报表中心 业务逻辑接口。
 */
public interface IReportService {

    /** 月访问量 — 对齐 J8 TrendChartVO（points + unit） */
    MonthlyAccessVO getMonthlyAccess(MonthlyAccessQuery query);

    /** 出入库金额对比 — 对齐 J8 StockInOutVO（categories + series + unit） */
    SupplyAmountCompareVO getSupplyAmountCompare(SupplyAmountCompareQuery query);

    /** 效期预警分布 — 对齐 J8 ExpireWarningVO（items + unit） */
    ExpirationWarningVO getExpirationWarning(ExpirationWarningQuery query);

    /** 周接诊量趋势 — 对齐 J8 TrendChartVO */
    EncounterWeekTrendVO getWeekTrend(EncounterWeekTrendQuery query);

    /** 月接诊量趋势 — 对齐 J8 TrendChartVO */
    EncounterMonthTrendVO getMonthTrend(EncounterMonthTrendQuery query);

    /** 疾病类型分布 — 对齐 J8 DistributionVO */
    DiseaseDistributionVO getDiseaseDistribution(DiseaseStatisticQuery query);
}
