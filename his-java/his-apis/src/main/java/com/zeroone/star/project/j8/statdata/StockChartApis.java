package com.zeroone.star.project.j8.statdata;

import com.zeroone.star.project.dto.j8.statdata.ExpireWarningDTO;
import com.zeroone.star.project.dto.j8.statdata.StockChartDTO;
import com.zeroone.star.project.query.j8.statdata.StockChartQuery;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

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
     * 获取近半年物资入库出库堆叠柱状图数据
     * @param query 查询参数（起始月份、结束月份、物资类型筛选）
     * @return ECharts堆叠柱状图数据（xAxis、series）
     */
    JsonVO<StockChartDTO> getStockInOutChart(StockChartQuery query);

    /**
     * 获取库存物品过期预警饼图数据
     * @param goodsType 物资分类（drug-药品/consumable-耗材/device-器械，不传默认全部）
     * @return 过期预警数据列表
     */
    JsonVO<List<ExpireWarningDTO>> getExpireWarningPie(String goodsType);
}