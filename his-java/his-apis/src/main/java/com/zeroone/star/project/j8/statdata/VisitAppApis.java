package com.zeroone.star.project.j8.statdata;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.HourTrendVO;

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
     * 获取小时访问趋势
     * @param query 查询参数（日期范围、粒度、组织ID）
     * @return 小时访问趋势
     */
    JsonVO<HourTrendVO> getHourTrend(HourTrendQuery query);

    /**
     * 获取月访问量
     * @param query 查询参数（日期范围、粒度、组织ID）
     * @return 月访问量趋势
     */
    JsonVO<HourTrendVO> getMonthVisitTrend(HourTrendQuery query);
}