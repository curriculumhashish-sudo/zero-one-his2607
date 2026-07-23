package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeWithdrawApprovalDTO;

public interface IStocktakeApprovalService {
    Boolean submit(StocktakeSubmitApprovalDTO dto);

    Boolean withdraw(StocktakeWithdrawApprovalDTO dto);

    Boolean agree(StocktakeAgreeApprovalDTO dto);

    Boolean reject(StocktakeRejectApprovalDTO dto);
}
