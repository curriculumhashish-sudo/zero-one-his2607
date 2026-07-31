package com.zeroone.star.report.service.impl;

import com.zeroone.star.report.mapper.AdmEncounterDiagnosisMapper;
import com.zeroone.star.report.mapper.AdmEncounterMapper;
import com.zeroone.star.report.service.IDataStatisticsService;
import com.zeroone.star.project.dto.j6.report.DiseaseDistributionDTO;
import com.zeroone.star.project.dto.j6.report.EncounterTrendDTO;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.time.YearMonth;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * 数据统计服务实现类
 * @author j6小组-TKORHP
 */
@Service
@RequiredArgsConstructor
public class DataStatisticsServiceImpl implements IDataStatisticsService {

    private final AdmEncounterMapper admEncounterMapper;
    private final AdmEncounterDiagnosisMapper admEncounterDiagnosisMapper;

    @Override
    public List<EncounterTrendDTO> queryWeekTrend(EncounterWeekTrendQuery query) {
        LocalDate startDate = query.getStartDate();
        LocalDate endDate = query.getEndDate();

        if (startDate == null && endDate == null) {
            // 场景1：两个都为空，以当天为准，补满当周（周一到周日）
            LocalDate today = LocalDate.now();
            startDate = today.with(DayOfWeek.MONDAY);
            endDate = today.with(DayOfWeek.SUNDAY);
        } else if (startDate != null && endDate == null) {
            // 场景2：只有开始日期，以开始日期为准，补满当周
            endDate = startDate.with(DayOfWeek.SUNDAY);
            startDate = startDate.with(DayOfWeek.MONDAY);
        } else if (startDate == null) {
            // 场景2：只有结束日期，以结束日期为准，补满当周
            startDate = endDate.with(DayOfWeek.MONDAY);
            endDate = endDate.with(DayOfWeek.SUNDAY);
        } else {
            // 场景3：两个都有值
            LocalDate startWeekMonday = startDate.with(DayOfWeek.MONDAY);
            LocalDate endWeekMonday = endDate.with(DayOfWeek.MONDAY);
            if (startWeekMonday.equals(endWeekMonday)) {
                // 在同一个星期内：补满那个星期（周一到周日）
                startDate = startWeekMonday;
                endDate = endDate.with(DayOfWeek.SUNDAY);
            } else {
                // 不在一个星期内：以结束日期往前补满7天
                startDate = endDate.minusDays(6);
            }
        }

        return queryDailyTrend(startDate, endDate);
    }

    @Override
    public List<EncounterTrendDTO> queryMonthTrend(EncounterMonthTrendQuery query) {
        LocalDate startDate = query.getStartDate();
        LocalDate endDate = query.getEndDate();

        if (startDate == null && endDate == null) {
            // 场景1：两个都为空，以当天为准，补满当月（1号到月末）
            LocalDate today = LocalDate.now();
            startDate = today.withDayOfMonth(1);
            endDate = today.withDayOfMonth(YearMonth.from(today).lengthOfMonth());
        } else if (startDate != null && endDate == null) {
            // 场景2：只有开始日期，以开始日期为准，补满当月
            endDate = startDate.withDayOfMonth(YearMonth.from(startDate).lengthOfMonth());
            startDate = startDate.withDayOfMonth(1);
        } else if (startDate == null) {
            // 场景2：只有结束日期，以结束日期为准，补满当月
            startDate = endDate.withDayOfMonth(1);
            endDate = endDate.withDayOfMonth(YearMonth.from(endDate).lengthOfMonth());
        } else {
            // 场景3：两个都有值，数据校验已确保在同一月内，补满该月
            startDate = startDate.withDayOfMonth(1);
            endDate = endDate.withDayOfMonth(YearMonth.from(endDate).lengthOfMonth());
        }

        return queryDailyTrend(startDate, endDate);
    }

    @Override
    public List<DiseaseDistributionDTO> queryDiseaseStatistic() {
        // 总就诊人数以 adm_encounter 为准
        int total = admEncounterMapper.selectTotalEncounterCount();

        // 查询各疾病类型分布（adm_encounter → adm_encounter_diagnosis → cln_condition → cln_condition_definition）
        List<Map<String, Object>> rawList = admEncounterDiagnosisMapper.selectDiseaseDistribution();

        // 转换为 DTO 并计算占比
        List<DiseaseDistributionDTO> result = new ArrayList<>();
        for (Map<String, Object> row : rawList) {
            DiseaseDistributionDTO dto = new DiseaseDistributionDTO();
            dto.setDiseaseName((String) row.get("diseaseName"));
            int count = ((Number) row.get("count")).intValue();
            dto.setCount(count);
            if (total > 0) {
                // 保留一位小数
                double percentage = Math.round(count * 1000.0 / total) / 10.0;
                dto.setPercentage(percentage);
            } else {
                dto.setPercentage(0.0);
            }
            result.add(dto);
        }
        return result;
    }

    /**
     * 查询每日接诊趋势并补齐缺失日期
     * 数据库只返回有数据的日期，对于没有接诊记录的日期补0
     * @param startDate 开始日期
     * @param endDate   结束日期
     * @return 每日接诊量趋势列表（缺失日期补0）
     */
    private List<EncounterTrendDTO> queryDailyTrend(LocalDate startDate, LocalDate endDate) {
        // 查询数据库获取有数据的日期
        List<Map<String, Object>> rawList = admEncounterMapper.selectDailyEncounterCount(startDate, endDate);

        // 转为 Map<日期字符串, 接诊人数>，便于后续查找
        Map<String, Integer> dateCountMap = rawList.stream()
                .collect(Collectors.toMap(
                        row -> (String) row.get("date"),
                        row -> ((Number) row.get("value")).intValue()
                ));

        // 遍历日期范围，补齐缺失日期（值为0）
        List<EncounterTrendDTO> result = new ArrayList<>();
        LocalDate current = startDate;
        while (!current.isAfter(endDate)) {
            String dateStr = current.toString();
            EncounterTrendDTO dto = new EncounterTrendDTO();
            dto.setDate(dateStr);
            dto.setValue(dateCountMap.getOrDefault(dateStr, 0));
            result.add(dto);
            current = current.plusDays(1);
        }
        return result;
    }
}
