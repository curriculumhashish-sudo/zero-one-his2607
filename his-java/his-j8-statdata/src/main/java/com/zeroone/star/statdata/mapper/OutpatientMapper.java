package com.zeroone.star.statdata.mapper;

import com.zeroone.star.project.vo.j8.statdata.DiseaseDistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.WeeklyVisitItemVO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;

import java.time.LocalDate;
import java.util.List;

@Mapper
public interface OutpatientMapper {

//    获取周门诊数据
    @Select("SELECT DATE(start_time) as date,count(*) as count " +
            "FROM adm_encounter " +
            "WHERE start_time >= #{monday}\n" +
            "AND start_time < #{sunday} " +
            "group by DATE(start_time)")
    List<WeeklyVisitItemVO> getWeekData(LocalDate monday, LocalDate sunday);

//    获取月门诊数据
    @Select("SELECT DATE(start_time) as date,count(*) as count " +
            "FROM adm_encounter " +
            "WHERE start_time >= #{start}\n" +
            "AND start_time < #{end} " +
            "group by DATE(start_time)")
    List<WeeklyVisitItemVO> getMonthData(LocalDate start, LocalDate end);

    /**
     * 统计门诊患者疾病类型分布
     */
    @Select("SELECT IF(d.description IS NULL OR d.description = '', '其他症状', d.description) AS diseaseName, " +
            "       COUNT(DISTINCT e.patient_id) AS count " +
            "FROM adm_encounter e " +
            "JOIN adm_encounter_diagnosis d ON e.id = d.encounter_id " +
            "WHERE e.is_deleted = 0 AND d.is_deleted = 0 " +
            "  AND e.class_enum = 'AMB' " +
            "GROUP BY IF(d.description IS NULL OR d.description = '', '其他症状', d.description) " +
            "ORDER BY count DESC")
    List<DiseaseDistributionItemVO> getDiseaseDistribution();
}
