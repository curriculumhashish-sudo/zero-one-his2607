package com.zeroone.star.statdata.service;

import com.zeroone.star.project.dto.j8.statdata.BaseStatQueryDTO;
import com.zeroone.star.project.vo.j8.statdata.DistributionVO;
import com.zeroone.star.project.vo.j8.statdata.TrendChartVO;

public interface OutpatientService {

    /**
     * 获取周门诊量趋势
     */
    TrendChartVO getWeeklyOutpatientData(BaseStatQueryDTO dto);

    /**
     * 获取月门诊量趋势
     */
    TrendChartVO getMonthlyOutpatientData(BaseStatQueryDTO dto);

    /**
     * 获取患者疾病类型分布
     */
    DistributionVO getDiseaseDistribution(BaseStatQueryDTO dto);
}
