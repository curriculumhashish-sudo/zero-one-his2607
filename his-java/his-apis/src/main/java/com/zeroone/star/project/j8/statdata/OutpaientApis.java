package com.zeroone.star.project.j8.statdata;

import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.DiseaseDistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.MonthlyVisitItemVO;
import com.zeroone.star.project.vo.j8.statdata.WeeklyVisitItemVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.GetMapping;

import java.util.ArrayList;
import java.util.List;

/**
 * 描述：报表中心-数据统计-门诊数据接口
 */
public interface OutpaientApis {

    /**
     * 获取周接诊量门诊趋势
     * @return 周接诊量门诊趋势对象
     */
    JsonVO<List<WeeklyVisitItemVO>> getVisitTrendWeekly(WeeklyVisitItemVO weeklyVisitItemVO);

    /**
     * 获取月接诊量门诊趋势
     * @return 月接诊量门诊趋势对象
     */
    JsonVO<List<MonthlyVisitItemVO>> getMonthlyVisitTrend(MonthlyVisitItemVO monthlyVisitItemVO);


    /**
     * 获取患者疾病类型分布
     * @return 患者疾病类型分布对象
     */
    JsonVO<List<DiseaseDistributionItemVO>> getDiseaseDistribution(DiseaseDistributionItemVO diseaseDistributionItemVO);
}
