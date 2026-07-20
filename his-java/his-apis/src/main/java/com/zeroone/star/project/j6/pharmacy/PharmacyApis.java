package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeDeleteDTO;
import com.zeroone.star.project.vo.JsonVO;

public interface PharmacyApis {
    default JsonVO<Boolean> submitStocktakeApproval(StocktakeSaveDTO dto) {
        return null;
    }

    default JsonVO<Boolean> withdrawStocktakeApproval(StocktakeDeleteDTO dto) {
        return null;
    }

    default JsonVO<Boolean> agreeStocktakeApproval(StocktakeDeleteDTO dto) {
        return null;
    }

    default JsonVO<Boolean> rejectStocktakeApproval(StocktakeDeleteDTO dto) {
        return null;
    }
}
