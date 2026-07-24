package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.WithdrawStockOutApprovalDTO;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;

public interface WarehouseStockOutApis {
    /**
     * 删除出库单
     * @param id 出库单ID
     * @return 是否成功
     */
    JsonVO<String> removeStockOut(String id);

    /**
     * 获取出库单详情
     * @param id 出库单ID
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
     * @param dto 提交审批参数（busNo）
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
     * @param dto 通过审批参数（busNo + 可选contractBusNo）
     * @return 操作结果（含操作后状态 + 药房入库壳单号）
     */
    JsonVO<StockOutApprovalResultVO> agreeStockOut(AgreeStockOutDTO dto);

    /**
     * 驳回出库审批
     * @param dto 驳回审批参数（busNo + rejectReason）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<StockOutApprovalResultVO> rejectStockOut(RejectStockOutDTO dto);
}
