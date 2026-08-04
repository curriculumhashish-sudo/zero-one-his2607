package com.zeroone.star.statdata.service.impl;

import com.zeroone.star.project.dto.j8.statdata.BaseStatQueryDTO;
import com.zeroone.star.project.vo.j8.statdata.DistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.DistributionVO;
import com.zeroone.star.project.vo.j8.statdata.TrendChartVO;
import com.zeroone.star.project.vo.j8.statdata.TrendPointVO;
import com.zeroone.star.statdata.mapper.AdmEncounterMapper;
import com.zeroone.star.statdata.service.OutpatientService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class OutpatientServiceImpl implements OutpatientService {

    private static final DateTimeFormatter DATE_FMT = DateTimeFormatter.ofPattern("yyyy-MM-dd");

    @Autowired
    private AdmEncounterMapper admEncounterMapper;

    @Override
    public TrendChartVO getWeeklyOutpatientData(BaseStatQueryDTO dto) {
        // 解析 dateFrom，不传则默认本周一
        LocalDate dateFrom = parseDate(dto.getDateFrom(), LocalDate.now().with(java.time.DayOfWeek.MONDAY));
        // 周范围：周一到周日（exclusive 边界为下周一的 00:00）
        LocalDate monday = dateFrom.with(java.time.temporal.TemporalAdjusters.previousOrSame(java.time.DayOfWeek.MONDAY));
        LocalDate sundayExclusive = monday.plusDays(7);

        List<TrendPointVO> rawData = admEncounterMapper.getWeekData(monday, sundayExclusive, dto.getOrganizationId());

        // 按日期索引
        Map<String, Integer> dataMap = rawData.stream()
                .collect(Collectors.toMap(TrendPointVO::getLabel, TrendPointVO::getValue, (a, b) -> a));

        // 补齐七天
        List<TrendPointVO> points = new ArrayList<>();
        String[] dayNames = {"周一", "周二", "周三", "周四", "周五", "周六", "周日"};
        for (int i = 0; i < 7; i++) {
            String dateLabel = monday.plusDays(i).format(DATE_FMT);
            points.add(new TrendPointVO(dayNames[i], dataMap.getOrDefault(dateLabel, 0)));
        }

        TrendChartVO result = new TrendChartVO();
        result.setPoints(points);
        result.setUnit("人次");
        return result;
    }

    @Override
    public TrendChartVO getMonthlyOutpatientData(BaseStatQueryDTO dto) {
        // 解析 dateFrom，不传则默认当月第一天
        LocalDate dateFrom = parseDate(dto.getDateFrom(), LocalDate.now().withDayOfMonth(1));
        LocalDate firstDay = dateFrom.withDayOfMonth(1);
        LocalDate lastDayExclusive = firstDay.plusMonths(1);

        List<TrendPointVO> rawData = admEncounterMapper.getMonthData(firstDay, lastDayExclusive, dto.getOrganizationId());

        // 按日期索引
        Map<String, Integer> dataMap = rawData.stream()
                .collect(Collectors.toMap(TrendPointVO::getLabel, TrendPointVO::getValue, (a, b) -> a));

        // 每两天一个区间
        List<TrendPointVO> points = new ArrayList<>();
        int daysOfMonth = firstDay.lengthOfMonth();
        for (int start = 1; start <= daysOfMonth; start += 2) {
            int end = Math.min(start + 1, daysOfMonth);
            int count = 0;
            for (int d = start; d <= end; d++) {
                String dateLabel = firstDay.withDayOfMonth(d).format(DATE_FMT);
                count += dataMap.getOrDefault(dateLabel, 0);
            }
            points.add(new TrendPointVO(start + "日-" + end + "日", count));
        }

        TrendChartVO result = new TrendChartVO();
        result.setPoints(points);
        result.setUnit("人次");
        return result;
    }

    @Override
    public DistributionVO getDiseaseDistribution(BaseStatQueryDTO dto) {
        // 解析日期范围，不传则默认当月
        LocalDate dateFrom = parseDate(dto.getDateFrom(), LocalDate.now().withDayOfMonth(1));
        LocalDate dateTo = parseDate(dto.getDateTo(), dateFrom.plusMonths(1));

        List<DistributionItemVO> items = admEncounterMapper.getDiseaseDistribution(dateFrom, dateTo, dto.getOrganizationId());

        DistributionVO result = new DistributionVO();
        result.setItems(items);
        result.setUnit("人");
        return result;
    }

    /**
     * 解析日期字符串，失败时返回默认值
     */
    private LocalDate parseDate(String dateStr, LocalDate defaultDate) {
        if (dateStr == null || dateStr.isBlank()) {
            return defaultDate;
        }
        try {
            return LocalDate.parse(dateStr, DATE_FMT);
        } catch (Exception e) {
            return defaultDate;
        }
    }
}
