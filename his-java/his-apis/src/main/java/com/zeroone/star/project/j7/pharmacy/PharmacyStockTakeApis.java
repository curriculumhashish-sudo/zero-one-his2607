package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;
import com.zeroone.star.project.vo.JsonVO;

public interface PharmacyStockTakeApis {

    /**
     * 提交审批
     */
    JsonVO<StocktakeStatusVO> submit(OperatorDTO dto);

    /**
     * 审批状态通过
     */
    JsonVO<StocktakeStatusVO> approve(ApproveDTO dto);
    /**
     * 驳回审批
     */
    JsonVO<StocktakeStatusVO> reject(ApproveDTO dto);
    /**
     * 撤回审批
     */
    JsonVO<StocktakeStatusVO> withdraw(OperatorDTO dto);
}
