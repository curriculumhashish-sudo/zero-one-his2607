package com.zeroone.star.statdata.service;

import com.zeroone.star.project.dto.j8.statdata.StockChartDTO;
import com.zeroone.star.project.query.j8.statdata.StockChartQuery;
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
     * 获取库存出入库图表数据
     * @param query 查询参数
     * @return 库存图表数据
     */
    StockChartDTO getStockChartDTO(StockChartQuery query);
}