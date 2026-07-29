package com.zeroone.star.project.j7.pharmacy;

import com.zeroone.star.project.dto.j7.pharmacy.PharmacyStockDTO;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;


public interface PharmacyApis {

    /**
     * 添加入库单据
     * POST /j7/pharmacy/addPharmacyStock
     */
    JsonVO<Long> addPharmacyStock(PharmacyStockDTO pharmacyStockDTO);

    /**
     * 保存入库单据
     * POST /j7/pharmacy/savePharmacyStocks
     */
    JsonVO<Long> savePharmacyStocks(List<PharmacyStockDTO> pharmacyStockDTOList);

    /**
     * 保存并提交入库单据
     * POST /j7/pharmacy/saveAndSubmitPharmacyStocks
     */
    JsonVO<Long> saveAndSubmitPharmacyStocks(List<PharmacyStockDTO> pharmacyStockDTOListO);
}
