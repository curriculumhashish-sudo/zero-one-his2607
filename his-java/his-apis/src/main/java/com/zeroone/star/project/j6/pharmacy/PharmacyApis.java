package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptSaveDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptSaveResultVO;

public interface PharmacyApis {
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
