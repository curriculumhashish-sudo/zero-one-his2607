package com.zeroone.star.statdata.service;

import com.zeroone.star.project.dto.j8.statdata.HourTrendDTO;
import com.zeroone.star.project.dto.j8.statdata.MonthTrendDTO;

import java.util.List;
/**
 * <p>
 * 访问趋势服务接口
 * </p>
 *
 * @author highxin
 * @since 2026-07-29
 */
public interface IVisitTrendService {

    List<HourTrendDTO> getHourTrend(String day, String tenantId, String hospitalId);

    List<MonthTrendDTO> getMonthTrend(String yearMonth, String tenantId, String hospitalId);
}