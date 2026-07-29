package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;

/**
 * G7 入库单审批流转服务。
 *
 * @author lvv
 * @version 1.0.0
 */
public interface IInboundApprovalService {

    /**
     * lvv：撤回待审批入库单。
     *
     * @param dto 流转参数
     * @return 流转结果
     */
    InboundApprovalVO withdrawApproval(InboundApprovalDTO dto);

    /**
     * lvv：审批通过待审批入库单。
     *
     * @param dto 流转参数
     * @return 流转结果
     */
    InboundApprovalVO approveApproval(InboundApprovalDTO dto);

    /**
     * lvv：驳回待审批入库单。
     *
     * @param dto 流转参数
     * @return 流转结果
     */
    InboundApprovalVO rejectApproval(InboundApprovalDTO dto);
}
