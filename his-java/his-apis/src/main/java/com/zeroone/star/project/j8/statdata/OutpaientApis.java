package com.zeroone.star.project.j8.statdata;

import com.zeroone.star.project.dto.j8.statdata.BaseStatQueryDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.DistributionVO;
import com.zeroone.star.project.vo.j8.statdata.TrendChartVO;
import io.swagger.v3.oas.annotations.Operation;
import org.springframework.web.bind.annotation.GetMapping;

/**
 * 报表中心-数据统计-门诊数据接口
 */
public interface OutpaientApis {

    /**
     * 获取周接诊量门诊趋势
     */
    @Operation(summary = "获取周接诊量门诊趋势")
    JsonVO<TrendChartVO> getVisitTrendWeekly(BaseStatQueryDTO dto);

    /**
     * 获取月接诊量门诊趋势
     */
    @Operation(summary = "获取月接诊量门诊趋势")
    JsonVO<TrendChartVO> getMonthlyVisitTrend(BaseStatQueryDTO dto);

    /**
     * 获取患者疾病类型分布
     */
    @Operation(summary = "获取患者疾病类型分布")
    JsonVO<DistributionVO> getDiseaseDistribution(BaseStatQueryDTO dto);
}
