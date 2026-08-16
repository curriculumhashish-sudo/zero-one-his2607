package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.stockout.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.WithdrawStockOutApprovalDTO;

/**
 * 药库出库审批 Service 接口
 */
public interface IStockoutApprovalService {

    /**
     * 提交出库审批
     * @param dto 提交审批参数（busNo + approverId）
     * @return 操作结果
     */
    Boolean submitStockOutApproval(SubmitStockOutApprovalDTO dto);

    /**
     * 撤回出库审批
     * @param dto 撤回审批参数（busNo）
     * @return 操作结果
     */
    Boolean withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto);

    /**
     * 通过出库审批
     * @param dto 通过审批参数（busNo + 可选contractBusNo）
     * @return 操作结果
     */
    Boolean agreeStockOut(AgreeStockOutDTO dto);

    /**
     * 驳回出库审批
     * @param dto 驳回审批参数（busNo + rejectReason）
     * @return 操作结果
     */
    Boolean rejectStockOut(RejectStockOutDTO dto);
}
