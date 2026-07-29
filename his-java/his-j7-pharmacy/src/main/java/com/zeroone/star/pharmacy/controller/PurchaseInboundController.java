package com.zeroone.star.pharmacy.controller;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/j7/pharmacy/purchaseInbound")
@RequiredArgsConstructor
@Tag(name = "药房管理-采购入库", description = "入库单查询与提交审批相关接口")
public class PurchaseInboundController {

    private final InStockOrderService inStockOrderService;

    @Operation(summary = "获取单据列表")
    @PostMapping("/list")
    public JsonVO<IPage<PurchaseInboundListVO>> list(@RequestBody PurchaseInboundQuery query) {
        return JsonVO.success(inStockOrderService.listInboundOrders(query));
    }

    @Operation(summary = "获取单据详情")
    @GetMapping("/detail/{id}")
    public JsonVO<PurchaseInboundDetailVO> detail(@PathVariable("id") String id) {
        return JsonVO.success(inStockOrderService.getInboundDetail(id));
    }

    @Operation(summary = "提交入库单审批")
    @PostMapping("/submit")
    public JsonVO<Void> submit(@RequestBody @Validated PurchaseInboundSubmitDTO dto) {
        inStockOrderService.submitInboundOrder(dto);
        return JsonVO.success(null);
    }
}