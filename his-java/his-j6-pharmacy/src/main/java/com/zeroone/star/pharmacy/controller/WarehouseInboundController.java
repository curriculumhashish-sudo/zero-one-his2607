package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IInboundReceiptApprovalService;
import com.zeroone.star.pharmacy.service.IInboundReceiptService;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.*;
import com.zeroone.star.project.j6.pharmacy.WarehouseInboundApis;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.inbound.InboundReceiptQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptListVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.List;

@RestController
@RequestMapping("/inbound")
@Tag(name = "药库进货单", description = "药库管理相关接口")
public class WarehouseInboundController implements WarehouseInboundApis {

    @Resource
    private IInboundReceiptApprovalService inboundApprovalService;

    @Resource
    private IInboundReceiptService inboundReceiptService;

    @Override
    @GetMapping("/list")
    @Operation(summary = "获取单据列表（条件+分页）")
    public JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query) {
        return JsonVO.success(inboundReceiptService.queryAllInboundReceipt(query));
    }

    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取单据详情")
    public JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query) {
        return JsonVO.success(inboundReceiptService.getInboundReceiptDetail(query));
    }

    @Override
    @DeleteMapping("/remove")
    @Operation(summary = "删除单据")
    public JsonVO<String> removeInboundReceipt(@Valid @RequestBody InboundReceiptDeleteDTO dto) {
        return JsonVO.success(inboundReceiptService.removeInboundReceipt(dto));
    }

    @Override
    @Operation(summary = "新增单据", description = "新增一张药库进货单草稿，后续数据落库由业务服务实现。")
    @PostMapping("/add")
    public JsonVO<InboundReceiptSaveResultVO> addInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "DRAFT", "新增成功", null, dto.getVersion()));
    }

    @Override
    @Operation(summary = "保存单据", description = "保存已存在的药库进货单草稿，后续数据落库由业务服务实现。")
    @PutMapping("save")
    public JsonVO<InboundReceiptSaveResultVO> saveInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "DRAFT", "保存成功", null, dto.getVersion()));
    }

    @Override
    @Operation(summary = "保存并提交审批", description = "保存单据并提交审批，提交后进入待审批状态，后续数据落库和审批流由业务服务实现。")
    @PostMapping("/submit")
    public JsonVO<InboundReceiptSaveResultVO> saveAndSubmitInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "PENDING_APPROVAL", "保存并提交审批成功", LocalDateTime.now(), dto.getVersion()));
    }

    private InboundReceiptSaveResultVO buildResult(String busNo, String statusEnum, String actionResult, LocalDateTime applyTime, Integer version) {
        InboundReceiptSaveResultVO result = new InboundReceiptSaveResultVO();
        result.setId("510000000000000001");
        result.setBusNo(busNo == null || busNo.isBlank() ? "JHD202607190001" : busNo);
        result.setStatusEnum(statusEnum);
        result.setVersion(version == null ? 0 : version + 1);
        result.setActionResult(actionResult);
        result.setApplyTime(applyTime);
        return result;
    }

    @Override
    @PostMapping("/submitInboundReceiptApproval")
    @Operation(
            summary = "提交审批",
            description = "将草稿（DRAFT）状态的进货单提交至审批流程，状态变更为待审批（PENDING_APPROVAL）。可指定审批人（approverId）。服务端写入 applicantId 和 applyTime。"
    )
    public JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(
            @Parameter(description = "提交审批入参", required = true)
            @RequestBody @Valid SubmitInboundReceiptApprovalDTO dto) {
        return JsonVO.success(inboundApprovalService.submit(dto));
    }

    @Override
    @PostMapping("/withdrawInboundReceiptApproval")
    @Operation(
            summary = "撤回审批",
            description = "仅单据提交人可执行此操作。"
    )
    public JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(
            @Parameter(description = "撤回审批入参", required = true)
            @RequestBody @Valid WithdrawInboundReceiptApprovalDTO dto) {
        return JsonVO.success(inboundApprovalService.withdraw(dto));
    }

    @Override
    @PostMapping("/agreeInboundReceipt")
    @Operation(
            summary = "审批通过",
            description = "审批人通过进货单，状态变更为已通过（AGREE）。服务端执行库存入库并写入 approverId 和 approvalTime。"
    )
    public JsonVO<InboundApprovalResultVO> agreeInboundReceipt(
            @Parameter(description = "审批通过入参", required = true)
            @RequestBody @Valid AgreeInboundReceiptDTO dto) {
        return JsonVO.success(inboundApprovalService.agree(dto));
    }

    @Override
    @PostMapping("/rejectInboundReceipt")
    @Operation(
            summary = "驳回审批",
            description = "审批人驳回进货单，状态变更为已驳回（REJECT）。必须填写驳回原因（rejectReason）。"
    )
    public JsonVO<InboundApprovalResultVO> rejectInboundReceipt(
            @Parameter(description = "驳回审批入参", required = true)
            @RequestBody @Valid RejectInboundReceiptDTO dto) {
        return JsonVO.success(inboundApprovalService.reject(dto));
    }
}
