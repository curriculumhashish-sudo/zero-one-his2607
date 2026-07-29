package com.zeroone.star.pharmacy.controller.end;

import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.*;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.j7.PharmacyApis;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.*;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@Tag(name = "药房盘点单", description = "药房盘点单基础流程")
public class yaofangpandiandan implements PharmacyApis {

    @Resource
    private IWkfSupplyRequestService wkfSupplyRequestService;

    @PutMapping("/submit")
    @Operation(summary = "提交审批")
    @Override
    public JsonVO<StocktakeStatusVO> submit(@RequestBody OperatorDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.submit(dto));
    }

    @PutMapping("/approve")
    @Operation(summary = "审批通过")
    @Override
    public JsonVO<StocktakeStatusVO> approve(@RequestBody ApproveDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.approve(dto));
    }

    @PutMapping("/reject")
    @Operation(summary = "审批驳回")
    @Override
    public JsonVO<StocktakeStatusVO> reject(@RequestBody ApproveDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.reject(dto));
    }

    @PutMapping("/withdraw")
    @Operation(summary = "撤回审批")
    @Override
    public JsonVO<StocktakeStatusVO> withdraw(@RequestBody OperatorDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.withdraw(dto));
    }

    @Override
    public JsonVO<InboundApprovalVO> withdrawApproval(InboundApprovalDTO dto) {
        return null;
    }

    @Override
    public JsonVO<InboundApprovalVO> approveApproval(InboundApprovalDTO dto) {
        return null;
    }

    @Override
    public JsonVO<InboundApprovalVO> rejectApproval(InboundApprovalDTO dto) {
        return null;
    }

    @Override
    public JsonVO<List<SummaryVO>> getSummaryList() {
        return null;
    }

    @Override
    public JsonVO<List<DrugItemVO>> getDrugItemsBySummary(String summaryId) {
        return null;
    }

    @Override
    public JsonVO<String> confirmDispensing(String dispensingId) {
        return null;
    }

    @Override
    public JsonVO<String> rejectDispensing(DispensingRejectDTO dto) {
        return null;
    }

    @Override
    public JsonVO<PageDTO<PatientQueueVO>> queryReturnPatientQueue(PatientQueueQuery query) {
        return null;
    }

    @Override
    public JsonVO<List<DrugItemVO>> getReturnDrugItems(String returnId) {
        return null;
    }

    @Override
    public JsonVO<String> confirmReturn(ReturnConfirmDTO dto) {
        return null;
    }

    @Override
    public JsonVO<PageDTO<PatientQueueVO>> queryOutpatientReturnQueue(PatientQueueQuery query) {
        return null;
    }

    @Override
    public JsonVO<String> rejectReturn(ReturnRejectDTO dto) {
        return null;
    }

    @Override
    public JsonVO<Long> addPharmacyStock(PharmacyStockDTO pharmacyStockDTO) {
        return null;
    }

    @Override
    public JsonVO<Long> savePharmacyStocks(List<PharmacyStockDTO> pharmacyStockDTOList) {
        return null;
    }

    @Override
    public JsonVO<Long> saveAndSubmitPharmacyStocks(List<PharmacyStockDTO> pharmacyStockDTOListO) {
        return null;
    }

}
