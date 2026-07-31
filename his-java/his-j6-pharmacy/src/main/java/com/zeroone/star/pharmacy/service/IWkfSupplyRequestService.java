package com.zeroone.star.pharmacy.service;

import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDeleteVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.PharmacyInventoryVO;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
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
    /**
     * 根据单据号逻辑删除盘点单据
     * @param busNo 单据号
     * @return 删除结果
     */
    PharmacyInventoryDeleteVO deleteStocktake(String busNo);

    /**
     * 分页多条件查询盘点单据列表
     * @param query 查询条件（分页、单据号、库房、日期）
     * @return 分页VO
     */
    PharmacyInventoryListVO listInventory(PharmacyInventoryQuery query);


    /**
     * 根据盘点单号查询单据详情（主信息+明细）
     * @param query 盘点业务单号
     * @return 盘点单据详情VO
     */
    PharmacyInventoryDetailVO getStocktakeDetail(PharmacyInventoryDetailQuery query);
}


