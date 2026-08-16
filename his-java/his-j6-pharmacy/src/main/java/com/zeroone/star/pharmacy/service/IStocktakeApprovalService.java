package com.zeroone.star.pharmacy.service;


import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeWithdrawApprovalDTO;

public interface IStocktakeApprovalService {
    Boolean submit(StocktakeSubmitApprovalDTO dto);

    Boolean withdraw(StocktakeWithdrawApprovalDTO dto);

    Boolean agree(StocktakeAgreeApprovalDTO dto);

    Boolean reject(StocktakeRejectApprovalDTO dto);
}
