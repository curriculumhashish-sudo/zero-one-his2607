package com.zeroone.star.project.j8.statdata;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.ExpireWarningVO;
import com.zeroone.star.project.vo.j8.statdata.StockInOutVO;

/**
 * <p>
 * 描述：库存统计图表接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author highxin
 * @version 1.0.0
 */
public interface StockChartApis {

    /**
     * 获取出入库堆叠柱状图
     * @param query 查询参数
     * @return 堆叠柱状图数据
     */
    JsonVO<StockInOutVO> getStockInOutChart(HourTrendQuery query);

    /**
     * 获取过期预警饼图
     * @param query 查询参数
     * @return 过期预警饼图数据
     */
    JsonVO<ExpireWarningVO> getExpireWarningPie(HourTrendQuery query);
}