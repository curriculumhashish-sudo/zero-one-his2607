package com.zeroone.star.report.service.impl;

import com.zeroone.star.project.dto.j6.report.DiseaseDistributionDTO;
import com.zeroone.star.project.dto.j6.report.EncounterTrendDTO;
import com.zeroone.star.project.query.j6.report.DiseaseStatisticQuery;
import com.zeroone.star.project.query.j6.report.EncounterMonthTrendQuery;
import com.zeroone.star.project.query.j6.report.EncounterWeekTrendQuery;
import com.zeroone.star.project.query.j6.report.ExpirationWarningQuery;
import com.zeroone.star.project.query.j6.report.MonthlyAccessQuery;
import com.zeroone.star.project.query.j6.report.SupplyAmountCompareQuery;
import com.zeroone.star.project.vo.j6.report.DiseaseDistributionVO;
import com.zeroone.star.project.vo.j6.report.DistributionItemVO;
import com.zeroone.star.project.vo.j6.report.EncounterMonthTrendVO;
import com.zeroone.star.project.vo.j6.report.EncounterWeekTrendVO;
import com.zeroone.star.project.vo.j6.report.ExpirationWarningVO;
import com.zeroone.star.project.vo.j6.report.MonthlyAccessVO;
import com.zeroone.star.project.vo.j6.report.SupplyAmountCompareVO;
import com.zeroone.star.project.vo.j6.report.TrendPointVO;
import com.zeroone.star.report.mapper.AdmEncounterDiagnosisMapper;
import com.zeroone.star.report.mapper.AdmEncounterMapper;
import com.zeroone.star.report.mapper.WhaleAuditLogsMapper;
import com.zeroone.star.report.mapper.WkfInventoryItemMapper;
import com.zeroone.star.report.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.report.service.IReportService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class ReportServiceImpl implements IReportService {

    @Resource
    private WhaleAuditLogsMapper whaleAuditLogsMapper;

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;

    @Resource
    private AdmEncounterMapper admEncounterMapper;

    @Resource
    private AdmEncounterDiagnosisMapper admEncounterDiagnosisMapper;

    @Override
    public MonthlyAccessVO getMonthlyAccess(MonthlyAccessQuery query) {
        Integer year = (query != null && query.getYear() != null) ? query.getYear() : 2026;
        List<Map<String, Object>> rawList = whaleAuditLogsMapper.selectMonthlyAccessCountRaw(year);

        Map<Integer, Long> countMap = rawList.stream()
                .collect(Collectors.toMap(
                        r -> ((Number) r.get("month")).intValue(),
                        r -> ((Number) r.get("count")).longValue(),
                        (v1, v2) -> v1));

        List<TrendPointVO> points = new ArrayList<>();
        for (int m = 1; m <= 12; m++) {
            points.add(new TrendPointVO(m + "月", countMap.getOrDefault(m, 0L).intValue()));
        }

        MonthlyAccessVO vo = new MonthlyAccessVO();
        vo.setPoints(points);
        vo.setUnit("次");
        return vo;
    }

    @Override
    public SupplyAmountCompareVO getSupplyAmountCompare(SupplyAmountCompareQuery query) {
        String start = query != null && query.getDateFrom() != null ? query.getDateFrom() : "2026-01-01";
        String end = query != null && query.getDateTo() != null ? query.getDateTo() : "2026-12-31";
        List<Map<String, Object>> rawList = wkfSupplyRequestMapper.selectSupplyAmountCompareRaw(start, end);

        // 聚合：period → 入库金额 / 出库金额
        Map<String, Double> inboundMap = new java.util.LinkedHashMap<>();
        Map<String, Double> outboundMap = new java.util.LinkedHashMap<>();
        for (Map<String, Object> row : rawList) {
            String period = (String) row.get("period");
            String type = (String) row.get("type");
            double amount = ((Number) row.get("totalAmount")).doubleValue();
            if ("INBOUND".equals(type)) {
                inboundMap.merge(period, amount, Double::sum);
            } else {
                outboundMap.merge(period, amount, Double::sum);
            }
        }

        List<String> categories = new ArrayList<>(inboundMap.keySet());
        for (String k : outboundMap.keySet()) {
            if (!categories.contains(k)) categories.add(k);
        }
        Collections.sort(categories);

        List<Double> inboundValues = new ArrayList<>();
        List<Double> outboundValues = new ArrayList<>();
        for (String cat : categories) {
            inboundValues.add(inboundMap.getOrDefault(cat, 0.0));
            outboundValues.add(outboundMap.getOrDefault(cat, 0.0));
        }

        SupplyAmountCompareVO.SupplySeriesItem inSeries = new SupplyAmountCompareVO.SupplySeriesItem();
        inSeries.setName("药品/耗材");
        inSeries.setDirection("入库");
        inSeries.setValues(inboundValues);

        SupplyAmountCompareVO.SupplySeriesItem outSeries = new SupplyAmountCompareVO.SupplySeriesItem();
        outSeries.setName("药品/耗材");
        outSeries.setDirection("出库");
        outSeries.setValues(outboundValues);

        SupplyAmountCompareVO vo = new SupplyAmountCompareVO();
        vo.setCategories(categories);
        vo.setSeries(List.of(inSeries, outSeries));
        vo.setUnit("元");
        return vo;
    }

    @Override
    public ExpirationWarningVO getExpirationWarning(ExpirationWarningQuery query) {
        Integer expireWithinDays = query != null && query.getExpireWithinDays() != null
                ? query.getExpireWithinDays() : 90;
        List<Map<String, Object>> rawList = wkfInventoryItemMapper.selectExpirationWarningRaw(expireWithinDays);

        List<DistributionItemVO> items = rawList.stream()
                .map(r -> new DistributionItemVO(
                        (String) r.get("name"),
                        ((Number) r.get("count")).intValue()))
                .collect(Collectors.toList());

        ExpirationWarningVO vo = new ExpirationWarningVO();
        vo.setItems(items);
        vo.setUnit("种");
        return vo;
    }

    @Override
    public EncounterWeekTrendVO getWeekTrend(EncounterWeekTrendQuery query) {
        List<EncounterTrendDTO> raw = queryDailyTrend(query.getStartDate(), query.getEndDate());
        List<TrendPointVO> points = raw.stream()
                .map(d -> new TrendPointVO(d.getDate(), d.getValue()))
                .collect(Collectors.toList());
        EncounterWeekTrendVO vo = new EncounterWeekTrendVO();
        vo.setPoints(points);
        vo.setUnit("人次");
        return vo;
    }

    @Override
    public EncounterMonthTrendVO getMonthTrend(EncounterMonthTrendQuery query) {
        List<EncounterTrendDTO> raw = queryDailyTrend(query.getStartDate(), query.getEndDate());
        List<TrendPointVO> points = raw.stream()
                .map(d -> new TrendPointVO(d.getDate(), d.getValue()))
                .collect(Collectors.toList());
        EncounterMonthTrendVO vo = new EncounterMonthTrendVO();
        vo.setPoints(points);
        vo.setUnit("人次");
        return vo;
    }

    @Override
    public DiseaseDistributionVO getDiseaseDistribution(DiseaseStatisticQuery query) {
        int total = admEncounterMapper.selectTotalEncounterCount();
        List<Map<String, Object>> rawList = admEncounterDiagnosisMapper.selectDiseaseDistribution();

        List<DistributionItemVO> items = new ArrayList<>();
        for (Map<String, Object> row : rawList) {
            String name = (String) row.get("diseaseName");
            int count = ((Number) row.get("count")).intValue();
            items.add(new DistributionItemVO(name, count));
        }

        DiseaseDistributionVO vo = new DiseaseDistributionVO();
        vo.setItems(items);
        vo.setUnit("人");
        return vo;
    }

    // ========== 辅助 ==========

    private List<EncounterTrendDTO> queryDailyTrend(java.time.LocalDate startDate, java.time.LocalDate endDate) {
        if (startDate == null) startDate = java.time.LocalDate.now().minusDays(30);
        if (endDate == null) endDate = java.time.LocalDate.now();
        List<Map<String, Object>> rawList = admEncounterMapper.selectDailyEncounterCount(startDate, endDate);
        Map<String, Integer> dateCountMap = rawList.stream()
                .collect(Collectors.toMap(
                        r -> (String) r.get("date"),
                        r -> ((Number) r.get("value")).intValue()));

        List<EncounterTrendDTO> result = new ArrayList<>();
        java.time.LocalDate current = startDate;
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
