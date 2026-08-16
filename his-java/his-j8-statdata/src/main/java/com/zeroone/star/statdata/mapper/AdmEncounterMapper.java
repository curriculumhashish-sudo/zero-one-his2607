package com.zeroone.star.statdata.mapper;

import com.zeroone.star.project.vo.j8.statdata.DistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.TrendPointVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.time.LocalDate;
import java.util.List;

@Mapper
public interface AdmEncounterMapper {

    /**
     * 获取门诊日趋势数据（按天分组）
     */
    List<TrendPointVO> getWeekData(@Param("dateFrom") LocalDate dateFrom,
                                   @Param("dateTo") LocalDate dateTo,
                                   @Param("organizationId") String organizationId);

    /**
     * 获取门诊月趋势原始日数据（Service 层拼区间）
     */
    List<TrendPointVO> getMonthData(@Param("dateFrom") LocalDate dateFrom,
                                    @Param("dateTo") LocalDate dateTo,
                                    @Param("organizationId") String organizationId);

    /**
     * 统计门诊患者疾病类型分布
     */
    List<DistributionItemVO> getDiseaseDistribution(@Param("dateFrom") LocalDate dateFrom,
                                                     @Param("dateTo") LocalDate dateTo,
                                                     @Param("organizationId") String organizationId);
}
