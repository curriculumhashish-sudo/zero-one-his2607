package com.zeroone.star.pharmacy.service;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeApprovalDTO;

public interface IStocktakeApprovalService {
    Boolean submit(StocktakeApprovalDTO dto);

    Boolean withdraw(StocktakeApprovalDTO dto);

    Boolean agree(StocktakeApprovalDTO dto);

    Boolean reject(StocktakeApprovalDTO dto);
}
