package com.zeroone.star.project.j8.statdata;

import com.zeroone.star.project.dto.j8.statdata.HourTrendDTO;
import com.zeroone.star.project.dto.j8.statdata.MonthTrendDTO;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

/**
 * <p>
 * 描述：统计数据接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author highxin
 * @version 1.0.0
 */
public interface VisitAppApis {

    /**
     * 获取当日各小时访问流量趋势
     * @param day 统计日期(yyyy-MM-dd)
     * @return 小时流量列表
     */
    JsonVO<List<HourTrendDTO>> getHourTrend(String day);

    /**
     * 获取当月每日访问量趋势
     * @param month 统计月份(yyyy-MM)
     * @return 月份流量列表
     */
    JsonVO<List<MonthTrendDTO>> getMonthVisitTrend(String month);
}