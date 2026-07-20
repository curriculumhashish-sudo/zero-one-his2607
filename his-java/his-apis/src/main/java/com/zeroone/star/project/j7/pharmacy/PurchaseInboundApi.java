package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
// 导入PurchaseInboundQuery
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.JsonVO;


import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;

import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.GetMapping;

import java.util.List;

// 药房管理-药房入库单（获取单据列表、获取单据详情、提交审批）
@Tag(name = "采购入库")
@RequestMapping("/purchaseInbound")
public interface PurchaseInboundApi {

    /**
     * 获取入库单列表
     */
    @Operation(summary = "获取入库单列表")
    @PostMapping("/list")
    JsonVO<List<PurchaseInboundListVO>> list(
            @RequestBody PurchaseInboundQuery query);

    /**
     * 获取入库单详情
     */
    @Operation(summary = "获取入库单详情")
    @GetMapping("/detail/{id}")
    JsonVO<PurchaseInboundDetailVO> detail(
            @PathVariable("id") Long id);

    /**
     * 提交审批
     */
    @Operation(summary = "提交审批")
    @PostMapping("/submit")
    JsonVO<Void> submit(
            @RequestBody PurchaseInboundSubmitDTO dto);

}