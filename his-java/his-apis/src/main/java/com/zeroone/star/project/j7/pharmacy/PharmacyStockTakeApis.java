package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;
import com.zeroone.star.project.vo.JsonVO;

public interface PharmacyStockTakeApis {

    /**
     * 盘点单提交审批
     */
    JsonVO<StocktakeStatusVO> submit(OperatorDTO dto);

    /**
     * 盘点单审批状态通过
     */
    JsonVO<StocktakeStatusVO> approve(ApproveDTO dto);
    /**
     * 盘点单驳回审批
     */
    JsonVO<StocktakeStatusVO> reject(ApproveDTO dto);
    /**
     * 盘点单撤回审批
     */
    JsonVO<StocktakeStatusVO> withdraw(OperatorDTO dto);
}
