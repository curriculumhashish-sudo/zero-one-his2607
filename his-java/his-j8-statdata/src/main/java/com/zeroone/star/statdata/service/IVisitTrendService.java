package com.zeroone.star.statdata.service;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.j8.statdata.HourTrendVO;

/**
 * <p>
 * 访问趋势服务接口
 * </p>
 *
 * @author highxin
 * @since 2026-07-29
 */
public interface IVisitTrendService {

    HourTrendVO getHourTrend(HourTrendQuery query);

    HourTrendVO getMonthTrend(HourTrendQuery query);
}