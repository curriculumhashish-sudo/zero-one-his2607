package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.j8.statdata.StockChartApis;
import com.zeroone.star.project.query.j8.statdata.HourTrendQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j8.statdata.ExpireWarningVO;
import com.zeroone.star.project.vo.j8.statdata.StockInOutVO;
import com.zeroone.star.statdata.service.IWkfInventoryItemService;
import com.zeroone.star.statdata.service.IWkfSupplyRequestService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>描述: 库存统计图表控制器</p>
 * <p>版权: &copy;01星球</p>
 * <p>地址: 01星球总部</p>
 *
 * @author highxin
 * @version 1.0.0
 */
@RestController
@RequestMapping("/j8/statdata") // 统一模块路径前缀
@Tag(name = "库存统计图表")
public class StockChartController implements StockChartApis {

    private final IWkfSupplyRequestService supplyRequestService;
    private final IWkfInventoryItemService inventoryItemService;

    public StockChartController(IWkfSupplyRequestService supplyRequestService, IWkfInventoryItemService inventoryItemService) {
        this.supplyRequestService = supplyRequestService;
        this.inventoryItemService = inventoryItemService;
    }

    @Override
    @Operation(summary = "获取出入库堆叠柱状图",
               description = "按月分组统计各类物资入库、出库金额")
    @GetMapping("/in-out")
    public JsonVO<StockInOutVO> getStockInOutChart(HourTrendQuery query) {
        return JsonVO.success(supplyRequestService.getStockChartDTO(query));
    }

    @Override
    @Operation(summary = "获取过期预警饼图",
               description = "统计近1个月过期、3个月内过期、安全期的库存物品数量")
    @GetMapping("/expire-warning")
    public JsonVO<ExpireWarningVO> getExpireWarningPie(HourTrendQuery query) {
        return JsonVO.success(inventoryItemService.getExpireWarningData(query));
    }
}