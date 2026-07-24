package com.zeroone.star.project.j6.report;

import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;
import com.zeroone.star.project.vo.JsonVO;
import java.util.List;

public interface DataStatisticsApis {
    /**
     * 获取月访问量
     * @param query 查询条件
     * @return 12个月的访问量趋势
     */
    JsonVO<List<MonthlyAccessVO>> queryMonthlyAccess(MonthlyAccessQuery query);

    /**
     * 物资入库与出库金额对比
     * @param query 查询条件
     * @return 出入库金额对比数据列表
     */
    JsonVO<List<SupplyAmountCompareVO>> querySupplyAmountCompare(SupplyAmountCompareQuery query);

    /**
     * 库存物资有效期预警分布
     * @param query 查询条件
     * @return 预警物资列表数据
     */
    JsonVO<List<ExpirationWarningVO>> queryExpirationWarning(ExpirationWarningQuery query);

    /**
     * 查询周接诊量门诊趋势
     * @param query
     * @return
     */
    JsonVO<EncounterWeekTrendVO> queryWeekTrend(EncounterWeekTrendQuery query);

    /**
     * 查询月接诊量门诊趋势
     * @param query
     * @return
     */
    JsonVO<EncounterMonthTrendVO> queryMonthTrend(EncounterMonthTrendQuery query);

    /**
     * 查询患者疾病类型分布
     * @return
     */
    JsonVO<DiseaseDistributionVO> queryDiseaseStatistic();


    /**
     * 获取流量趋势
     * @param query 查询条件（日期范围、统计粒度）
     * @return 流量趋势数据点列表
     */
    JsonVO<List<TrafficTrendVO>> queryTrafficTrend(TrafficTrendQuery query);
}