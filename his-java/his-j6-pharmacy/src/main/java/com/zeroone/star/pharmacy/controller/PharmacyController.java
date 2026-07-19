package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptSaveDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptSaveResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;

@RestController
@RequestMapping("/j6/pharmacy/inbound-receipt")
@Tag(name = "药库进货单", description = "药库进货单新增、保存、保存并提交审批接口")
public class PharmacyController implements PharmacyApis {
    @Override
    @Operation(summary = "新增药库进货单", description = "新增一张药库进货单草稿，后续数据落库由业务服务实现。")
    @PostMapping
    public JsonVO<InboundReceiptSaveResultVO> addInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "DRAFT", "新增成功", null));
    }

    @Override
    @Operation(summary = "保存药库进货单", description = "保存已存在的药库进货单草稿，后续数据落库由业务服务实现。")
    @PutMapping
    public JsonVO<InboundReceiptSaveResultVO> saveInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "DRAFT", "保存成功", null));
    }

    @Override
    @Operation(summary = "保存并提交药库进货单审批", description = "保存单据并提交审批，提交后进入待审批状态，后续数据落库和审批流由业务服务实现。")
    @PostMapping("/submit")
    public JsonVO<InboundReceiptSaveResultVO> saveAndSubmitInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "PENDING_APPROVAL", "保存并提交审批成功", LocalDateTime.now()));
    }

    private InboundReceiptSaveResultVO buildResult(String busNo, String statusEnum, String actionResult, LocalDateTime applyTime) {
        InboundReceiptSaveResultVO result = new InboundReceiptSaveResultVO();
        result.setBusNo(busNo == null || busNo.isBlank() ? "JHD202607190001" : busNo);
        result.setStatusEnum(statusEnum);
        result.setActionResult(actionResult);
        result.setApplyTime(applyTime);
        return result;
    }
}
