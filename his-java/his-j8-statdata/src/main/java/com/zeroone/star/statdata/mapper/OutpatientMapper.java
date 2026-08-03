package com.zeroone.star.statdata.mapper;

import com.zeroone.star.project.vo.j8.statdata.DistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.TrendPointVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.time.LocalDate;
import java.util.List;

@Mapper
public interface OutpatientMapper {

    /**
     * 获取门诊日趋势数据（按天分组）
     */
    @Select("<script>" +
            "SELECT DATE(e.start_time) AS label, " +
            "       COUNT(*) AS value " +
            "FROM adm_encounter e " +
            "WHERE e.is_deleted = 0 " +
            "  AND e.class_enum = 'AMB' " +
            "  AND e.start_time &gt;= #{dateFrom} " +
            "  AND e.start_time &lt; #{dateTo} " +
            "  <if test='organizationId != null and organizationId != \"\"'>" +
            "  AND e.organization_id = #{organizationId} " +
            "  </if>" +
            "GROUP BY DATE(e.start_time) " +
            "ORDER BY label" +
            "</script>")
    List<TrendPointVO> getWeekData(@Param("dateFrom") LocalDate dateFrom,
                                   @Param("dateTo") LocalDate dateTo,
                                   @Param("organizationId") String organizationId);

    /**
     * 获取门诊月趋势原始日数据（Service 层拼区间）
     */
    @Select("<script>" +
            "SELECT DATE(e.start_time) AS label, " +
            "       COUNT(*) AS value " +
            "FROM adm_encounter e " +
            "WHERE e.is_deleted = 0 " +
            "  AND e.class_enum = 'AMB' " +
            "  AND e.start_time &gt;= #{dateFrom} " +
            "  AND e.start_time &lt; #{dateTo} " +
            "  <if test='organizationId != null and organizationId != \"\"'>" +
            "  AND e.organization_id = #{organizationId} " +
            "  </if>" +
            "GROUP BY DATE(e.start_time) " +
            "ORDER BY label" +
            "</script>")
    List<TrendPointVO> getMonthData(@Param("dateFrom") LocalDate dateFrom,
                                    @Param("dateTo") LocalDate dateTo,
                                    @Param("organizationId") String organizationId);

    /**
     * 统计门诊患者疾病类型分布
     */
    @Select("<script>" +
            "SELECT IF(d.description IS NULL OR d.description = '', '其他症状', d.description) AS name, " +
            "       COUNT(DISTINCT e.patient_id) AS value " +
            "FROM adm_encounter e " +
            "JOIN adm_encounter_diagnosis d ON e.id = d.encounter_id " +
            "WHERE e.is_deleted = 0 AND d.is_deleted = 0 " +
            "  AND e.class_enum = 'AMB' " +
            "  AND e.start_time &gt;= #{dateFrom} " +
            "  AND e.start_time &lt; #{dateTo} " +
            "  <if test='organizationId != null and organizationId != \"\"'>" +
            "  AND e.organization_id = #{organizationId} " +
            "  </if>" +
            "GROUP BY IF(d.description IS NULL OR d.description = '', '其他症状', d.description) " +
            "ORDER BY value DESC" +
            "</script>")
    List<DistributionItemVO> getDiseaseDistribution(@Param("dateFrom") LocalDate dateFrom,
                                                     @Param("dateTo") LocalDate dateTo,
                                                     @Param("organizationId") String organizationId);
}
