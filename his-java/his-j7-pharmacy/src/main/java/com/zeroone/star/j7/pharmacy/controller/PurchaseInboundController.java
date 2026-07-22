package com.zeroone.star.j7.pharmacy.controller;


import com.zeroone.star.j7.pharmacy.service.PurchaseInboundService;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.j7.pharmacy.PurchaseInboundApi;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/j7/pharmacy/purchaseInbound")
@Validated
@Tag(name = "药房管理-采购入库", description = "入库单查询与提交审批相关接口")
@RequiredArgsConstructor
public class PurchaseInboundController implements PurchaseInboundApi {

    private final PurchaseInboundService purchaseInboundService;

    /* ==================== POST ==================== */

    @Operation(summary = "获取入库单列表", description = "支持按日期、状态等条件筛选入库单")
    @PostMapping("/list")
    @Override
    public JsonVO<List<PurchaseInboundListVO>> list(@RequestBody PurchaseInboundQuery query) {
        List<PurchaseInboundListVO> list = purchaseInboundService.listInboundOrders(query);
        if (list == null || list.isEmpty()) {
            return JsonVO.success(null);
        }
        return JsonVO.success(list);
    }

    @Operation(summary = "提交审批", description = "将草稿状态的入库单提交至审批流")
    @PostMapping("/submit")
    @Override
    public JsonVO<Void> submit(@RequestBody PurchaseInboundSubmitDTO dto) {
        // 假实现：目前不写真实业务逻辑
        return JsonVO.fail("该方法由 PurchaseInboundService 提供真实实现");
    }

    /* ==================== GET ==================== */

    @Operation(summary = "获取入库单详情", description = "根据ID获取入库单表头及明细信息")
    @GetMapping("/detail/{id}")
    @Override
    public JsonVO<PurchaseInboundDetailVO> detail(@PathVariable("id") Long id) {
        PurchaseInboundDetailVO detail = purchaseInboundService.getInboundDetail(id);
        if (detail == null) {
            return JsonVO.fail("未查询到该入库单详情");
        }
        return JsonVO.success(detail);
    }
}