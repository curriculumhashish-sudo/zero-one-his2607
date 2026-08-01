package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
/**
 * G7 入库单审批流转接口。
 *
 * @author lvv
 * @version 1.0.0
 */
public interface InboundApprovalApis {
    /**
     * 撤回待审批的入库单（POST /j7/inbound-approval/withdraw）。
     * 仅原申请人可操作，成功状态为 WITHDRAWN。
     *
     * @param dto 入库单审批流转请求参数
     * @return 操作结果
     */
    JsonVO<InboundApprovalVO> withdrawApproval(InboundApprovalDTO dto);
    /**
     * 审批通过入库单（POST /j7/inbound-approval/approve）。
     * 仅处理 PENDING_APPROVAL 状态，成功状态为 APPROVED。
     *
     * @param dto 入库单审批流转请求参数
     * @return 操作结果
     */
    JsonVO<InboundApprovalVO> approveApproval(InboundApprovalDTO dto);
    /**
     * 驳回入库单审批（POST /j7/inbound-approval/reject）。
     * 驳回原因必填，成功状态为 REJECTED。
     *
     * @param dto 入库单审批流转请求参数
     * @return 操作结果
     */
    JsonVO<InboundApprovalVO> rejectApproval(InboundApprovalDTO dto);
}

