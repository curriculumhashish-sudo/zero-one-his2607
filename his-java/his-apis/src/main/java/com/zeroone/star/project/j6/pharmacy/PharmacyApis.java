package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundApprovalResultVO;

/**
 * <p>
 * 描述：药库管理 — 进货单审批接口
 * </p>
 * @author AB
 * @version 1.0.0
 */
public interface PharmacyApis {

    /**
     * 提交审批
     * 将草稿状态的进货单提交至审批流程，状态由 DRAFT → PENDING_APPROVAL
     * @param dto 审批入参（必填 busNo；可选 applyClock，不传则服务端写入当前时间）
     * @return 操作结果（success + 最新状态 PENDING_APPROVAL）
     */
    JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(InboundReceiptApprovalDTO dto);

    /**
     * 撤回审批
     * 将待审批状态的进货单撤回到草稿状态，状态由 PENDING_APPROVAL → DRAFT
     * @param dto 审批入参（必填 busNo；其余字段忽略）
     * @return 操作结果（success + 最新状态 DRAFT）
     */
    JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(InboundReceiptApprovalDTO dto);

    /**
     * 审批通过
     * 审批人通过进货单，状态由 PENDING_APPROVAL → AGREE
     * @param dto 审批入参（必填 busNo；可选 approverId、approvalClock、contractBusNo）
     * @return 操作结果（success + 最新状态 AGREE）
     */
    JsonVO<InboundApprovalResultVO> agreeInboundReceipt(InboundReceiptApprovalDTO dto);

    /**
     * 驳回审批
     * 审批人驳回进货单，状态由 PENDING_APPROVAL → REJECT
     * @param dto 审批入参（必填 busNo、rejectReason；可选 approverId、approvalClock）
     * @return 操作结果（success + 最新状态 REJECT）
     */
    JsonVO<InboundApprovalResultVO> rejectInboundReceipt(InboundReceiptApprovalDTO dto);
}
