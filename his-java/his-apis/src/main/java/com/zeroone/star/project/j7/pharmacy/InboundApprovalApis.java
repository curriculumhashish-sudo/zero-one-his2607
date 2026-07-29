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
     * 撤回待审批的入库单。
     *
     * @param dto 入库单审批流转请求参数
     * @return 操作结果
     */
    JsonVO<InboundApprovalVO> withdrawApproval(InboundApprovalDTO dto);
    /**
     * 审批通过入库单。
     *
     * @param dto 入库单审批流转请求参数
     * @return 操作结果
     */
    JsonVO<InboundApprovalVO> approveApproval(InboundApprovalDTO dto);
    /**
     * 驳回入库单审批。
     *
     * @param dto 入库单审批流转请求参数
     * @return 操作结果
     */
    JsonVO<InboundApprovalVO> rejectApproval(InboundApprovalDTO dto);
}