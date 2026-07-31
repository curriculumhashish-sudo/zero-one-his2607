package com.zeroone.star.report.service.impl;

import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;
import com.zeroone.star.report.mapper.AdmEncounterMapper;
import com.zeroone.star.report.service.ITrafficTrendService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * 门诊流量趋势 Service 实现
 * @author J6
 */
@Service
public class TrafficTrendServiceImpl implements ITrafficTrendService {

    @Resource
    private AdmEncounterMapper admEncounterMapper;

    @Override
    public List<TrafficTrendVO> queryTrafficTrend(TrafficTrendQuery query) {
        if (!granularityValid(query.getGranularity())) {
            throw new RuntimeException("时间粒度只能为DAY/WEEK/MONTH");
        }

        return admEncounterMapper.selectTrafficTrend(
                query.getStart(),
                query.getEnd(),
                query.getGranularity() != null ? query.getGranularity() : "DAY"
        );
    }

    private boolean granularityValid(String granularity) {
        if (granularity != null &&   !granularity.isEmpty() ) {
            return granularity.equals("DAY") ||  granularity.equals("MONTH") || granularity.equals("WEEK");
        }

        return true;
    }
}
