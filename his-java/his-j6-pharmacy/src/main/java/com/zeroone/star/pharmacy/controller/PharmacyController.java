package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawInboundReceiptApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundApprovalResultVO;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;

import jakarta.validation.Valid;

import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：药库相关接口实现
 * </p>
 * @author AB
 * @version 1.0.0
 */
@RestController
@RequestMapping("j6/pharmacy")
@Tag(name = "药库进货单审批", description = "进货单的提交审批、撤回审批、审批通过、驳回审批四个审批流转操作")
public class PharmacyController implements PharmacyApis {

    // ============================================================
    // 提交审批
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/submitInboundReceiptApproval")
    @Operation(
        summary = "提交审批",
        description = "将草稿（DRAFT）状态的进货单提交至审批流程，状态变更为待审批（PENDING_APPROVAL）。可指定审批人（approverId）。服务端写入 applicantId 和 applyTime。"
    )
    public JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(
            @Parameter(description = "提交审批入参", required = true)
            @RequestBody @Valid SubmitInboundReceiptApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "APPROVAL"));
    }

    // ============================================================
    // 撤回审批
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/withdrawInboundReceiptApproval")
    @Operation(
        summary = "撤回审批",
        description = "将待审批（PENDING_APPROVAL）状态的进货单撤回到草稿（DRAFT）状态。仅单据提交人可执行此操作。"
    )
    public JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(
            @Parameter(description = "撤回审批入参", required = true)
            @RequestBody @Valid WithdrawInboundReceiptApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "WITHDRAW"));
    }

    // ============================================================
    // 审批通过
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/agreeInboundReceipt")
    @Operation(
        summary = "审批通过",
        description = "审批人通过进货单，状态变更为已通过（AGREE）。服务端写入 approverId 和 approvalTime。如需上传医保，可传入 contractBusNo。"
    )
    public JsonVO<InboundApprovalResultVO> agreeInboundReceipt(
            @Parameter(description = "审批通过入参", required = true)
            @RequestBody @Valid AgreeInboundReceiptDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "AGREE"));
    }

    // ============================================================
    // 驳回审批
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/rejectInboundReceipt")
    @Operation(
        summary = "驳回审批",
        description = "审批人驳回进货单，状态变更为已驳回（REJECT）。必须填写驳回原因（rejectReason）。服务端写入 approverId 和 approvalTime。"
    )
    public JsonVO<InboundApprovalResultVO> rejectInboundReceipt(
            @Parameter(description = "驳回审批入参", required = true)
            @RequestBody @Valid RejectInboundReceiptDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "REJECT"));
    }
}
