package com.zeroone.star.project.j6.report;

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

/**
 * 数据统计接口 — 对齐 J8 对应接口字段结构。
 * 路径保留 J6 前缀，文档中标注 J8 对应关系。
 */
public interface DataStatisticsApis {

    /**
     * 获取月访问量 — 对应 J8 getMonthlyVisitTrend
     */
    JsonVO<MonthlyAccessVO> queryMonthlyAccess(MonthlyAccessQuery query);

    /**
     * 物资出入库金额对比 — 对应 J8 getStockInOutChart
     */
    JsonVO<SupplyAmountCompareVO> querySupplyAmountCompare(SupplyAmountCompareQuery query);

    /**
     * 库存物资有效期预警分布 — 对应 J8 getExpireWarningPie
     */
    JsonVO<ExpirationWarningVO> queryExpirationWarning(ExpirationWarningQuery query);

    /**
     * 周接诊量门诊趋势 — 对应 J8 getVisitTrendWeekly
     */
    JsonVO<EncounterWeekTrendVO> queryWeekTrend(EncounterWeekTrendQuery query);

    /**
     * 月接诊量门诊趋势 — 对应 J8 getMonthlyVisitTrend
     */
    JsonVO<EncounterMonthTrendVO> queryMonthTrend(EncounterMonthTrendQuery query);

    /**
     * 患者疾病类型分布 — 对应 J8 getDiseaseDistribution
     */
    JsonVO<DiseaseDistributionVO> queryDiseaseStatistic(DiseaseStatisticQuery query);

    /**
     * 流量趋势 — 对应 J8 getHourTrend
     */
    JsonVO<TrafficTrendVO> queryTrafficTrend(TrafficTrendQuery query);
}
