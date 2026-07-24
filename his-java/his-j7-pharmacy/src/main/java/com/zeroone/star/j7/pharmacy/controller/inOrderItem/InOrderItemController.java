package com.zeroone.star.j7.pharmacy.controller.inOrderItem;

import com.zeroone.star.j7.pharmacy.service.IPharmacyService;
import com.zeroone.star.project.dto.j7.pharmacy.PharmacyStockDTO;
import com.zeroone.star.project.j7.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/j7/pharmacy")
@Tag(name = "药房管理")
public class InOrderItemController implements PharmacyApis {

    @Resource
    private IPharmacyService pharmacyService;

    @Override
    @Operation(summary = "新增入库单据")
    @PostMapping("/addPharmacyStock")
    public JsonVO<Long> addPharmacyStock(@RequestBody PharmacyStockDTO pharmacyStockDTO) {
        return null;
    }

    @Override
    @Operation(summary = "保存入库单据")
    @PostMapping("/savePharmacyStocks")
    public JsonVO<Long> savePharmacyStocks(@RequestBody List<PharmacyStockDTO> pharmacyStockDTOList) {

        return null;
    }

    @Override
    @Operation(summary = "保存并提交审批")
    @PostMapping("/saveAndSubmitPharmacyStocks")
    public JsonVO<Long> saveAndSubmitPharmacyStocks(@RequestBody List<PharmacyStockDTO> pharmacyStockDTOList) {

        return null;
    }
}