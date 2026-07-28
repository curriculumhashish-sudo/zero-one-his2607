package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IInboundReceiptApprovalService;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptDeleteDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.WithdrawInboundReceiptApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.WarehouseInboundApis;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/inbound")
@Tag(name = "药库进货单", description = "药库管理相关接口")
public class WarehouseInboundController implements WarehouseInboundApis {

    @Resource
    private IWkfSupplyRequestService supplyRequestService;

    @Resource
    private IInboundReceiptApprovalService inboundApprovalService;

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取单据列表")
    public JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query) {
        // TODO 调用 Service 查询数据库，返回列表
        return null;
    }

    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取单据详情")
    public JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query) {
        // TODO 调用 Service 查询详情，返回头和明细行
        return null;
    }

    @Override
    @DeleteMapping("/remove")
    @Operation(summary = "删除单据")
    public JsonVO<String> removeInboundReceipt(@Valid @RequestBody InboundReceiptDeleteDTO dto) {
        // TODO 调用 Service 执行删除
        return null;
    }

    @Override
    @PostMapping("/add")
    @Operation(summary = "新增单据", description = "新增一张药库进货单草稿，按明细行写入 wkf_supply_request。")
    public JsonVO<InboundReceiptSaveResultVO> addInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return executeSave(() -> supplyRequestService.addInboundReceipt(dto));
    }

    @Override
    @PutMapping("/save")
    @Operation(summary = "保存单据", description = "保存已存在的药库进货单草稿，校验版本后重建单据明细。")
    public JsonVO<InboundReceiptSaveResultVO> saveInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return executeSave(() -> supplyRequestService.saveInboundReceipt(dto));
    }

    @Override
    @PostMapping("/submit")
    @Operation(summary = "保存并提交审批", description = "保存单据并提交审批，提交后进入待审批状态并写入申请人、申请时间。")
    public JsonVO<InboundReceiptSaveResultVO> saveAndSubmitInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return executeSave(() -> supplyRequestService.saveAndSubmitInboundReceipt(dto));
    }

    @Override
    @PostMapping("/submitInboundReceiptApproval")
    @Operation(
            summary = "提交审批",
            description = "将草稿状态的进货单提交至审批流程，状态变更为待审批。"
    )
    public JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(
            @Parameter(description = "提交审批入参", required = true)
            @RequestBody @Valid SubmitInboundReceiptApprovalDTO dto) {
        return JsonVO.success(inboundApprovalService.submit(dto));
    }

    @Override
    @PostMapping("/withdrawInboundReceiptApproval")
    @Operation(summary = "撤回审批", description = "撤回待审批状态的进货单。")
    public JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(
            @Parameter(description = "撤回审批入参", required = true)
            @RequestBody @Valid WithdrawInboundReceiptApprovalDTO dto) {
        return JsonVO.success(inboundApprovalService.withdraw(dto));
    }

    @Override
    @PostMapping("/agreeInboundReceipt")
    @Operation(summary = "审批通过", description = "审批通过进货单，并执行库存入库。")
    public JsonVO<InboundApprovalResultVO> agreeInboundReceipt(
            @Parameter(description = "审批通过入参", required = true)
            @RequestBody @Valid AgreeInboundReceiptDTO dto) {
        return JsonVO.success(inboundApprovalService.agree(dto));
    }

    @Override
    @PostMapping("/rejectInboundReceipt")
    @Operation(summary = "驳回审批", description = "驳回进货单审批。")
    public JsonVO<InboundApprovalResultVO> rejectInboundReceipt(
            @Parameter(description = "驳回审批入参", required = true)
            @RequestBody @Valid RejectInboundReceiptDTO dto) {
        return JsonVO.success(inboundApprovalService.reject(dto));
    }

    private JsonVO<InboundReceiptSaveResultVO> executeSave(InboundReceiptSaveAction action) {
        try {
            return JsonVO.success(action.execute());
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @FunctionalInterface
    private interface InboundReceiptSaveAction {
        InboundReceiptSaveResultVO execute();
    }
}
