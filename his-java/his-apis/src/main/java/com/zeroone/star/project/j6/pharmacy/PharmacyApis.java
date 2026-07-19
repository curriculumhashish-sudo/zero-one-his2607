package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeApprovalDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface PharmacyApis {
    default JsonVO<Boolean> submitStocktakeApproval(StocktakeApprovalDTO dto) {
        return null;
    }

    default JsonVO<Boolean> withdrawStocktakeApproval(StocktakeApprovalDTO dto) {
        return null;
    }

    default JsonVO<Boolean> agreeStocktakeApproval(StocktakeApprovalDTO dto) {
        return null;
    }

    default JsonVO<Boolean> rejectStocktakeApproval(StocktakeApprovalDTO dto) {
        return null;
    }
}
