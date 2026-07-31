package com.zeroone.star.statdata.service.serviceImpl;

import com.zeroone.star.project.vo.j8.statdata.DiseaseDistributionItemVO;
import com.zeroone.star.project.vo.j8.statdata.MonthlyVisitItemVO;
import com.zeroone.star.project.vo.j8.statdata.WeeklyVisitItemVO;
import com.zeroone.star.statdata.mapper.OutpatientMapper;
import com.zeroone.star.statdata.service.OutpatientService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class OutpatientServiceImpl implements OutpatientService {


    @Autowired
    private OutpatientMapper outpatientMapper;



    @Override
    public List<WeeklyVisitItemVO> getWeeklyOutpatientData(LocalDate date) {
        //获取周一和周日的日期
        LocalDate monday = date.with(java.time.temporal.TemporalAdjusters.previousOrSame(java.time.DayOfWeek.MONDAY));
        LocalDate sunday = date.with(java.time.temporal.TemporalAdjusters.nextOrSame(java.time.DayOfWeek.SUNDAY)).plusDays(1);
        //获取对应日期的门诊量数据
        List<WeeklyVisitItemVO> weekData = outpatientMapper.getWeekData(monday, sunday);
        //排序
        Map<LocalDate,Integer> dataMap =
                weekData.stream()
                        .collect(Collectors.toMap(
                                WeeklyVisitItemVO::getDate,
                                WeeklyVisitItemVO::getCount
                        ));
                //补齐七天数据
        List<WeeklyVisitItemVO> result = new ArrayList<>();
        for (int i = 0; i < 7; i++) {
            LocalDate day = monday.plusDays(i);
            result.add(new WeeklyVisitItemVO(day, i+1, dataMap.getOrDefault(day, 0)));
        }
            return result;
    }

    @Override
    public List<MonthlyVisitItemVO> getMonthlyOutpatientData(LocalDate date) {
        // 获取当月第一天和最后一天（最后一天 + 1 天作为 SQL 的 exclusive 边界）
        LocalDate firstDay = date.withDayOfMonth(1);
        LocalDate lastDayExclusive = firstDay.plusMonths(1);
        // 获取对应月份的门诊量数据
        List<WeeklyVisitItemVO> monthData = outpatientMapper.getMonthData(firstDay, lastDayExclusive);
        // 按日期汇总
        Map<LocalDate, Integer> dataMap =
                monthData.stream()
                        .collect(Collectors.toMap(
                                WeeklyVisitItemVO::getDate,
                                WeeklyVisitItemVO::getCount
                        ));
        // 补齐整月数据，每两天作为一个区间
        List<MonthlyVisitItemVO> result = new ArrayList<>();
        int daysOfMonth = date.lengthOfMonth();
        for (int start = 1; start <= daysOfMonth; start += 2) {
            int end = Math.min(start + 1, daysOfMonth);
            int count = 0;
            for (int d = start; d <= end; d++) {
                count += dataMap.getOrDefault(firstDay.withDayOfMonth(d), 0);
            }
            String label = start + "日";
            String dateRange = start + "-" + end;
            result.add(new MonthlyVisitItemVO(label, dateRange, count));
        }
        return result;
    }

    @Override
    public List<DiseaseDistributionItemVO> getDiseaseDistribution() {
        List<DiseaseDistributionItemVO> diseaseDistribution = outpatientMapper.getDiseaseDistribution();
        // 获取患者总数
        int sum = diseaseDistribution.stream().mapToInt(DiseaseDistributionItemVO::getCount).sum();
        // 计算占比，保留两位小数
        for (DiseaseDistributionItemVO item : diseaseDistribution) {
            double percentage = item.getCount().doubleValue() / sum * 100;
            item.setPercentage(Math.round(percentage * 100.0) / 100.0);
        }
        return diseaseDistribution;
    }
}
