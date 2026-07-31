package com.zeroone.star.statdata.service;

import com.zeroone.star.project.vo.j8.statdata.DiseaseDistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.MonthlyVisitItemVO;
import com.zeroone.star.project.vo.j8.statdata.WeeklyVisitItemVO;

import java.time.LocalDate;
import java.util.List;

public interface OutpatientService {
    /**
     * 获取周门诊数据
     */
    List<WeeklyVisitItemVO> getWeeklyOutpatientData(LocalDate date);

    /**
     * 获取月门诊数据
     */
    List<MonthlyVisitItemVO> getMonthlyOutpatientData(LocalDate date);

    /**
     * 获取患者疾病类型分布
     */
    List<DiseaseDistributionItemVO> getDiseaseDistribution();
}
