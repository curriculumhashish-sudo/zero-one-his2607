package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.inbound.*;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;

import java.util.List;

public interface WarehouseInboundApis {
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

    /**
     * 展示表单全部信息
     * 展示出所有待审批或者正在审批或是已完成审批的表单信息
     * @param query 查询条件
     * @return 所有进货单列表
     */
    JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(InboundReceiptQuery query);

    /**
     * 展示选中表单的全部信息
     * 可以查看所有表单选中之后的列表
     * @param query 表单详情
     * @return 具体的表单数值
     */
    JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(InboundReceiptDetailQuery query);

    /**
     * 删除表单
     * @param dto 选中表单的字段
     * @return 删除是否成功
     */
    JsonVO<String> removeInboundReceipt(InboundReceiptDeleteDTO dto);

    /**
     * 新增药库进货单。
     * @param dto 新增数据
     * @return 新增结果
     */
    JsonVO<InboundReceiptSaveResultVO> addInboundReceipt(InboundReceiptSaveDTO dto);

    /**
     * 保存药库进货单。
     * @param dto 保存数据
     * @return 保存结果
     */
    JsonVO<InboundReceiptSaveResultVO> saveInboundReceipt(InboundReceiptSaveDTO dto);

    /**
     * 保存并提交药库进货单审批。
     * @param dto 保存并提交数据
     * @return 提交结果
     */
    JsonVO<InboundReceiptSaveResultVO> saveAndSubmitInboundReceipt(InboundReceiptSaveDTO dto);
}
