package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.inbound.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.WithdrawInboundReceiptApprovalDTO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundApprovalResultVO;

/**
 * <p>进货单审批业务接口</p>
 *
 * @author AB
 * @since 2026-07-27
 */
public interface IInboundReceiptApprovalService {

    /**
     * 提交审批：DRAFT / WITHDRAW / REJECT → PENDING_APPROVAL
     */
    InboundApprovalResultVO submit(SubmitInboundReceiptApprovalDTO dto);

    /**
     * 撤回审批：PENDING_APPROVAL → WITHDRAW
     */
    InboundApprovalResultVO withdraw(WithdrawInboundReceiptApprovalDTO dto);

    /**
     * 审批通过：PENDING_APPROVAL → AGREE（含库存入库）
     */
    InboundApprovalResultVO agree(AgreeInboundReceiptDTO dto);

    /**
     * 驳回审批：PENDING_APPROVAL → REJECT
     */
    InboundApprovalResultVO reject(RejectInboundReceiptDTO dto);
}
