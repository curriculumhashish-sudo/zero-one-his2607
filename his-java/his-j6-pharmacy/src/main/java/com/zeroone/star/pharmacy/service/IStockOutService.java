package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutAddDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveRequestDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;

/**
 * 药库出库单服务接口
 */
public interface IStockOutService {

    /**
     * 新增药库出库单草稿。
     * @param dto 新增上下文（源药库、目的药房等）
     * @return 完整的空白文档结构
     */
    StockOutDetailVO addStockOut(StockOutAddDTO dto);

    /**
     * 保存药库出库单草稿。
     * @param dto 保存数据（与新增返回结构一致）
     * @return 更新后的完整文档结构
     */
    StockOutDetailVO saveStockOut(StockOutSaveRequestDTO dto);

    /**
     * 保存并提交出库审批。
     * @param dto 保存并提交数据（与新增返回结构一致）
     * @return 操作结果
     */
    StockOutApprovalResultVO saveAndSubmitApproval(StockOutSaveRequestDTO dto);

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
