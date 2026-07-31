package com.zeroone.star.report.service;

import com.zeroone.star.project.query.j6.report.TrafficTrendQuery;
import com.zeroone.star.project.vo.j6.report.TrafficTrendVO;

import java.util.List;

/**
 * 门诊流量趋势 Service
 * @author J6
 */
public interface ITrafficTrendService {

    /**
     * 按时间粒度统计门诊数量
     * @param query 查询条件（日期范围 + 时间粒度）
     * @return 趋势数据点列表
     */
    List<TrafficTrendVO> queryTrafficTrend(TrafficTrendQuery query);
}
