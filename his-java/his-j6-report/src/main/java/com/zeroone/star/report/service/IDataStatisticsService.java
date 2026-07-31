package com.zeroone.star.report.service;

import com.zeroone.star.project.dto.j6.report.DiseaseDistributionDTO;
import com.zeroone.star.project.dto.j6.report.EncounterTrendDTO;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;

import java.util.List;

/**
 * 数据统计服务接口
 * @author j6小组-TKORHP
 */
public interface IDataStatisticsService {

    /**
     * 查询周接诊量门诊趋势
     * 日期补齐规则：
     * 两个都为空：以当天为准，补满当周（周一到周日）
     * 只有一个日期：以该日期为准，补满当周
     * 两个都有值且在同一周内：补满该周；不在同一周内：以结束日期往前补满7天
     * @param query 查询条件（日期范围，可选）
     * @return 每日接诊量趋势列表（缺失日期补0）
     */
    List<EncounterTrendDTO> queryWeekTrend(EncounterWeekTrendQuery query);

    /**
     * 查询月接诊量门诊趋势
     * 日期补齐规则：
     * 两个都为空：以当天为准，补满当月（1号到月末）
     * 只有一个日期：以该日期为准，补满当月
     * 两个都有值：数据校验已确保在同一月内，补满该月
     * @param query 查询条件（日期范围，可选）
     * @return 每日接诊量趋势列表（缺失日期补0）
     */
    List<EncounterTrendDTO> queryMonthTrend(EncounterMonthTrendQuery query);

    /**
     * 查询患者疾病类型分布
     * 总就诊人数以 adm_encounter 为准，关联到 adm_encounter_diagnosis → cln_condition → cln_condition_definition
     * @return 疾病类型分布列表
     */
    List<DiseaseDistributionDTO> queryDiseaseStatistic();
}
