package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
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

    /**
     * 删除草稿单据（逻辑删除）
     * @param busNo 单据编号
     * @return 是否成功
     */
    String deleteDraftByBusNo(String busNo);

    /**
     * 获取单据详情
     * @param busNo 单据编号
     * @return 单据详情VO
     */
    StockOutDetailVO getDetailByBusNo(String busNo);

    /**
     * 获取单据列表（条件+分页）
     * @param query 查询条件
     * @return 分页结果
     */
    PageDTO<StockOutListVO> getPharmacyList(StockOutQuery query);
}