package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawInboundReceiptApprovalDTO;
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
     * 提交审批：DRAFT → PENDING_APPROVAL
     */
    JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(SubmitInboundReceiptApprovalDTO dto);

    /**
     * 撤回审批：PENDING_APPROVAL → DRAFT
     */
    JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(WithdrawInboundReceiptApprovalDTO dto);

    /**
     * 审批通过：PENDING_APPROVAL → AGREE
     */
    JsonVO<InboundApprovalResultVO> agreeInboundReceipt(AgreeInboundReceiptDTO dto);

    /**
     * 驳回审批：PENDING_APPROVAL → REJECT
     */
    JsonVO<InboundApprovalResultVO> rejectInboundReceipt(RejectInboundReceiptDTO dto);
}
