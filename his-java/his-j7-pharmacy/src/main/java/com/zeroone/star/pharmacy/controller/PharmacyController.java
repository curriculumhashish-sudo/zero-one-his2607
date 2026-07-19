package com.zeroone.star.pharmacy.controller;


import com.zeroone.star.project.dto.j7.pharmacy.PharmacyStockDTO;
import com.zeroone.star.project.j7.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/j7/pharmacy")
@Tag(name = "药房管理")
public class PharmacyController implements PharmacyApis {

    @Operation(summary = "新增入库单据")
    @PostMapping("/addPharmacyStock")
    @Override
    public JsonVO<Long> addPharmacyStock(@RequestBody PharmacyStockDTO pharmacyStockDTO) {
        return null;
    }

    @Operation(summary = "保存入库单据")
    @PostMapping("/savePharmacyStock")
    @Override
    public JsonVO<Long> savePharmacyStocks(@RequestBody List<PharmacyStockDTO> pharmacyStockDTOList) {
        return null;
    }

    @Operation(summary = "保存并提交审批")
    @PostMapping("/saveAndSubmitPharmacyStock")
    @Override
    public JsonVO<Long> saveAndSubmitPharmacyStocks(@RequestBody List<PharmacyStockDTO> pharmacyStockDTOList) {
        return null;
    }
}
