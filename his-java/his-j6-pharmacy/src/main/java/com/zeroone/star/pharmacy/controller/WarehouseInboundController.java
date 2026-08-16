package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IInboundReceiptApprovalService;
import com.zeroone.star.pharmacy.service.IInboundReceiptService;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptAddDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptDeleteDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveRequestDTO;
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

    @Resource
    private IInboundReceiptService inboundReceiptService;

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取单据列表（条件+分页）")
    public JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query) {
        return JsonVO.success(inboundReceiptService.queryAllInboundReceipt(query));
    }

    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取单据详情")
    public JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query) {
        return JsonVO.success(inboundReceiptService.getInboundReceiptDetail(query));
    }

    @Override
    @DeleteMapping("/remove")
    @Operation(summary = "删除单据")
    public JsonVO<String> removeInboundReceipt(@Valid @RequestBody InboundReceiptDeleteDTO dto) {
        return JsonVO.success(inboundReceiptService.removeInboundReceipt(dto));
    }

    @Override
    @PostMapping("/add")
    @Operation(summary = "新增单据", description = "发送简洁上下文（库房、供应商等），返回空白草稿的完整文档结构（空卡片+空明细）。")
    public JsonVO<InboundReceiptDetailVO> addInboundReceipt(@Valid @RequestBody InboundReceiptAddDTO dto) {
        try {
            return JsonVO.success(supplyRequestService.addInboundReceipt(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PutMapping("/save")
    @Operation(summary = "保存单据", description = "保存已存在的药库进货单草稿（请求结构同新增返回），校验版本后重建单据明细。")
    public JsonVO<InboundReceiptDetailVO> saveInboundReceipt(@Valid @RequestBody InboundReceiptSaveRequestDTO dto) {
        try {
            return JsonVO.success(supplyRequestService.saveInboundReceipt(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PostMapping("/submit")
    @Operation(summary = "保存并提交审批", description = "保存单据并提交审批（请求结构同新增返回），提交后进入待审批状态。")
    public JsonVO<InboundReceiptDetailVO> saveAndSubmitInboundReceipt(@Valid @RequestBody InboundReceiptSaveRequestDTO dto) {
        try {
            return JsonVO.success(supplyRequestService.saveAndSubmitInboundReceipt(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
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

}
