package com.zeroone.star.statdata.service;

import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.j8.statdata.StockInOutVO;
import com.zeroone.star.statdata.entity.WkfSupplyRequest;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 库存出入库图表服务接口
 * </p>
 *
 * @author highxin
 * @since 2026-07-26
 */
public interface IWkfSupplyRequestService extends IService<WkfSupplyRequest> {
    /**
     * 获取出入库堆叠柱状图数据
     * @param query 查询参数
     * @return 堆叠柱状图数据
     */
    StockInOutVO getStockChartDTO(HourTrendQuery query);
}