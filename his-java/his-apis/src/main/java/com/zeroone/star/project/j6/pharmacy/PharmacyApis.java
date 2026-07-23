package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeWithdrawApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;

/**
 * 药库盘点审批接口
 */
public interface PharmacyApis {
    /**
     * 提交药库盘点审批
     *
     * @param dto 提交审批参数
     * @return JsonVO<Boolean> 提交结果
     */
    JsonVO<Boolean> submitStocktakeApproval(StocktakeSubmitApprovalDTO dto);

    /**
     * 撤回药库盘点审批
     *
     * @param dto 撤回审批参数
     * @return JsonVO<Boolean> 撤回结果
     */
    JsonVO<Boolean> withdrawStocktakeApproval(StocktakeWithdrawApprovalDTO dto);

    /**
     * 通过药库盘点审批
     *
     * @param dto 审批通过参数
     * @return JsonVO<Boolean> 审批结果
     */
    JsonVO<Boolean> agreeStocktakeApproval(StocktakeAgreeApprovalDTO dto);

    /**
     * 驳回药库盘点审批
     *
     * @param dto 审批驳回参数
     * @return JsonVO<Boolean> 驳回结果
     */
    JsonVO<Boolean> rejectStocktakeApproval(StocktakeRejectApprovalDTO dto);
}
