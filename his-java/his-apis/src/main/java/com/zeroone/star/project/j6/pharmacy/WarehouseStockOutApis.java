package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.*;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.RequestBody;

public interface WarehouseStockOutApis {

    /**
     * 新增药库出库单。
     * 发送简洁上下文，返回左侧空卡片数据 + 右侧空明细表格结构。
     * @param dto 新增上下文（源药库、目的药房等）
     * @return 完整的空白文档结构
     */
    JsonVO<StockOutDetailVO> addStockOut(@Valid @RequestBody StockOutAddDTO dto);

    /**
     * 保存出库单（编辑已有草稿）。
     * 请求参数与新增返回响应结构一致。
     * @param dto 保存数据
     * @return 更新后的完整文档结构
     */
    JsonVO<StockOutDetailVO> saveStockOut(@Valid @RequestBody StockOutSaveRequestDTO dto);

    /**
     * 保存并提交出库审批。
     * 请求参数与新增返回响应结构一致。
     * @param dto 保存并提交数据
     * @return 操作结果
     */
    JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(@Valid @RequestBody StockOutSaveRequestDTO dto);

    /**
     * 删除出库单
     * @param id 出库单busNo
     * @return 是否成功
     */
    JsonVO<String> removeStockOut(String id);

    /**
     * 获取出库单详情
     * @param id 出库单busNo
     * @return 详情
     */
    JsonVO<StockOutDetailVO> getStockOutDetail(String id);

    /**
     * 分页、条件查询出库单列表
     * @param stockOutQuery 查询条件
     * @return 分页结果
     */
    JsonVO<PageDTO<StockOutListVO>> getPharmacy(StockOutQuery stockOutQuery);

    /**
     * 提交出库审批
     * @param dto 提交审批参数（busNo + approverId）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<StockOutApprovalResultVO> submitStockOutApproval(SubmitStockOutApprovalDTO dto);

    /**
     * 撤回出库审批
     * @param dto 撤回审批参数（busNo）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<StockOutApprovalResultVO> withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto);

    /**
     * 通过出库审批
     * @param dto 通过审批参数（busNo + approverId + approvalClock + 可选contractBusNo）
     * @return 操作结果（含操作后状态 + 药房入库壳单号）
     */
    JsonVO<StockOutApprovalResultVO> agreeStockOut(AgreeStockOutDTO dto);

    /**
     * 驳回出库审批
     * @param dto 驳回审批参数（busNo + approverId + approvalClock + rejectReason）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<StockOutApprovalResultVO> rejectStockOut(RejectStockOutDTO dto);
}
