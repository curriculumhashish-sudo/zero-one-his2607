package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptSaveDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptSaveResultVO;
import io.swagger.v3.oas.annotations.Operation;

public interface PharmacyApis {
    /**
     * 新增药库进货单。
     * @param dto 新增数据
     * @return 新增结果
     */
    @Operation(summary = "新增药库进货单", description = "新增药库进货单草稿，返回业务单号、供应状态和版本号等前端交互字段。")
    JsonVO<InboundReceiptSaveResultVO> addInboundReceipt(InboundReceiptSaveDTO dto);

    /**
     * 保存药库进货单。
     * @param dto 保存数据
     * @return 保存结果
     */
    @Operation(summary = "保存药库进货单", description = "保存药库进货单草稿，保存已有单据时传入 busNo 和 version。")
    JsonVO<InboundReceiptSaveResultVO> saveInboundReceipt(InboundReceiptSaveDTO dto);

    /**
     * 保存并提交药库进货单审批。
     * @param dto 保存并提交数据
     * @return 提交结果
     */
    @Operation(summary = "保存并提交药库进货单审批", description = "保存药库进货单并提交审批，提交后返回待审批状态和申请时间。")
    JsonVO<InboundReceiptSaveResultVO> saveAndSubmitInboundReceipt(InboundReceiptSaveDTO dto);
}
