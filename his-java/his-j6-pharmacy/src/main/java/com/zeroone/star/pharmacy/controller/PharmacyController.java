package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

@RestController
@Tag(name = "pharmacy", description = "药库出库单接口")
@RequestMapping("/inventory/warehouse/stock-out")
public class PharmacyController implements PharmacyApis {

    @Operation(summary = "删除出库单")
    @DeleteMapping("/removeStockOut")
    @Override
    public JsonVO<String> removeStockOut(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "获取药库出库单详情")
    @GetMapping("/getStockOutDetail")
    @Override
    public JsonVO<StockOutDetailVO> getStockOutDetail(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "获取药库出库单列表")
    @GetMapping("/getPharmacy")
    @Override
    public JsonVO<PageDTO<StockOutListVO>> getPharmacy(@RequestBody StockOutQuery stockOutQuery) {
        return null;
    }
}
