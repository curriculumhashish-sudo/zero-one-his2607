package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutSaveResultVO;

/**
 * 药库出库单服务接口
 */
public interface IStockOutService {
    /**
     * 保存并提交审批
     */
    JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(StockOutSaveDTO dto);
    /**
     * 保存出库单
     */
    JsonVO<StockOutSaveResultVO> saveStockOut(StockOutSaveDTO dto);

    /**
     * 提交审批
     */
    JsonVO<StockOutApprovalResultVO> submitApproval(SubmitStockOutApprovalDTO dto);
}