package com.zeroone.star.report.service;

import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;

public interface ITrafficTrendService {
    TrafficTrendVO queryTrafficTrend(TrafficTrendQuery query);
}
