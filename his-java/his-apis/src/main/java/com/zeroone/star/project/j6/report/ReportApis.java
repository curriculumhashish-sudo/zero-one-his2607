package com.zeroone.star.project.j6.report;

import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import com.zeroone.star.project.vo.JsonVO

/**
 * <p>
 * 描述：报表管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j6小组-TKORHP
 * @version 1.0.0
 */
public interface ReportApis {
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
}
