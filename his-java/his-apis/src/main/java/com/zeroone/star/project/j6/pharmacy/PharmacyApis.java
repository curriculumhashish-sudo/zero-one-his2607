package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeWithdrawApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface PharmacyApis {
    default JsonVO<Boolean> submitStocktakeApproval(StocktakeSubmitApprovalDTO dto) {
        return null;
    }

    default JsonVO<Boolean> withdrawStocktakeApproval(StocktakeWithdrawApprovalDTO dto) {
        return null;
    }

    default JsonVO<Boolean> agreeStocktakeApproval(StocktakeAgreeApprovalDTO dto) {
        return null;
    }

    default JsonVO<Boolean> rejectStocktakeApproval(StocktakeRejectApprovalDTO dto) {
        return null;
    }
}
