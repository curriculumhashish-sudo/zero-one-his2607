package com.zeroone.star.statdata.controller;

import com.zeroone.star.project.dto.j8.statdata.ExpireWarningDTO;
import com.zeroone.star.project.dto.j8.statdata.StockChartDTO;
import com.zeroone.star.project.j8.statdata.StockChartApis;
import com.zeroone.star.project.query.j8.statdata.StockChartQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.statdata.service.IWkfInventoryItemService;
import com.zeroone.star.statdata.service.IWkfSupplyRequestService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;
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
               description = "按月分组统计各类物资入库、出库金额，返回适配ECharts的xAxis、series结构数据。若不传参数，自动获取当前时间所在半年的数据(上半年1-6月/下半年7-12月)")
    @GetMapping("/in-out")
    public JsonVO<StockChartDTO> getStockInOutChart(StockChartQuery query) {
        return JsonVO.success(supplyRequestService.getStockChartDTO(query));
    }

    @Operation(summary = "获取过期预警饼图", 
               description = "统计近1个月过期、3个月内过期、安全期的库存物品数量")
    @GetMapping("/getExpireWarningPie/{goodsType}")
    @Override
    public JsonVO<List<ExpireWarningDTO>> getExpireWarningPie(
            @Parameter(description = "物资分类：drug-药品/consumable-耗材/device-器械/all-全部", 
                       example = "drug", 
                       required = false)
            @PathVariable(required = false) String goodsType) {
        return JsonVO.success(inventoryItemService.getExpireWarningData(goodsType));
    }
}