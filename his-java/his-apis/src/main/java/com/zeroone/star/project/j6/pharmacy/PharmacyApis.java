package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawStockOutApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * <p>
 * 描述：药库出库审批API接口定义
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j6小组-告别
 * @version 1.0.0
 */
public interface PharmacyApis {

    /**
     * 提交出库审批
     * @param dto 提交审批参数（busNo）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<String> submitStockOutApproval(SubmitStockOutApprovalDTO dto);

    /**
     * 撤回出库审批
     * @param dto 撤回审批参数（busNo）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<String> withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto);

    /**
     * 通过出库审批
     * @param dto 通过审批参数（busNo + 可选contractBusNo）
     * @return 操作结果（含操作后状态 + 药房入库壳单号）
     */
    JsonVO<String> agreeStockOut(AgreeStockOutDTO dto);

    /**
     * 驳回出库审批
     * @param dto 驳回审批参数（busNo + rejectReason）
     * @return 操作结果（含操作后状态）
     */
    JsonVO<String> rejectStockOut(RejectStockOutDTO dto);
}
