package com.zeroone.star.report.service.impl;

import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;
import com.zeroone.star.project.vo.j6.report.TrendPointVO;
import com.zeroone.star.report.mapper.AdmEncounterMapper;
import com.zeroone.star.report.service.ITrafficTrendService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class TrafficTrendServiceImpl implements ITrafficTrendService {

    @Resource
    private AdmEncounterMapper admEncounterMapper;

    @Override
    public TrafficTrendVO queryTrafficTrend(TrafficTrendQuery query) {
        String start = query.getDateFrom() != null ? query.getDateFrom() : "2026-01-01";
        String end = query.getDateTo() != null ? query.getDateTo() : "2026-12-31";
        String granularity = query.getGranularity() != null ? query.getGranularity() : "DAY";

        List<Map<String, Object>> rawList = admEncounterMapper.selectTrafficTrendRaw(start, end, granularity);

        List<TrendPointVO> points = rawList.stream()
                .map(r -> new TrendPointVO(
                        (String) r.get("date"),
                        ((Number) r.get("count")).intValue()))
                .collect(Collectors.toList());

        TrafficTrendVO vo = new TrafficTrendVO();
        vo.setPoints(points);
        vo.setUnit("次");
        return vo;
    }
}
