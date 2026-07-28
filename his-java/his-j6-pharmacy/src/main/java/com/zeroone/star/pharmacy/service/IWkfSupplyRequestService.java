package com.zeroone.star.pharmacy.service;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;

/**
 * <p>
 * 工作流 supply request 服务类
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface IWkfSupplyRequestService extends IService<WkfSupplyRequest> {

    /**
     * 新增药库进货单草稿。
     *
     * @param dto 新增数据
     * @return 新增结果
     */
    InboundReceiptSaveResultVO addInboundReceipt(InboundReceiptSaveDTO dto);

    /**
     * 保存药库进货单草稿。
     *
     * @param dto 保存数据
     * @return 保存结果
     */
    InboundReceiptSaveResultVO saveInboundReceipt(InboundReceiptSaveDTO dto);

    /**
     * 保存药库进货单并提交审批。
     *
     * @param dto 保存并提交数据
     * @return 提交结果
     */
    InboundReceiptSaveResultVO saveAndSubmitInboundReceipt(InboundReceiptSaveDTO dto);
}
