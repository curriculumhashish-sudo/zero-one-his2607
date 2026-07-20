package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawInboundReceiptApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundApprovalResultVO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.StocktakeDetailVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.StockOutListVO;
import org.springframework.web.bind.annotation.*;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;
import com.zeroone.star.project.dto.j6.pharmacy.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.WithdrawStockOutApprovalDTO;

import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import lombok.RequiredArgsConstructor;
import io.swagger.v3.oas.annotations.Parameter;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.RequestMapping;

@RestController
@RequestMapping("j6/warehouse")
@RequiredArgsConstructor
@Tag(name = "药库管理", description = "药库管理相关接口")
public class PharmacyController implements PharmacyApis {


    @Override
    public JsonVO<StocktakeDetailVO> createStocktake(StocktakeSaveDTO dto) {
        return JsonVO.success(buildExampleDetailVO());
    }

    @Override
    public JsonVO<StocktakeDetailVO> saveStocktake(String busNo, StocktakeSaveDTO dto) {
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        return JsonVO.success(vo);
    }

    @Override
    public JsonVO<StocktakeDetailVO> submitStocktake(String busNo, StocktakeSaveDTO dto) {
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum("PENDING_APPROVAL");
        return JsonVO.success(vo);
    }

    /** Knife4j 示例数据 */
    private StocktakeDetailVO buildExampleDetailVO() {
        StocktakeDetailVO vo = new StocktakeDetailVO();
        vo.setBusNo("PD202607180001");
        vo.setStatusEnum("DRAFT");
        vo.setPurposeLocationId("284503862625177600");
        vo.setPurposeLocationName("西药库");
        vo.setPractitionerId("322733572165734400");
        vo.setApplicantId("322733572165734400");
        vo.setApplyTime(LocalDateTime.of(2026, 7, 18, 14, 0, 0));
        vo.setApproverId("322733572165734401");
        vo.setApprovalTime(LocalDateTime.of(2026, 7, 18, 15, 0, 0));

        StocktakeDetailVO.StocktakeDetailLineVO line =
                new StocktakeDetailVO.StocktakeDetailLineVO();
        line.setLineId("line_001");
        line.setItemTable("medication");
        line.setItemId("med_001");
        line.setProductName("阿莫西林胶囊");
        line.setSpecification("0.25g*24粒");
        line.setManufacturer("华北制药");
        line.setLotNumber("LOT20260701");
        line.setExpirationDate("2028-06-30");
        line.setUnitCode("PCS");
        line.setBookQuantity(new BigDecimal("95.00"));
        line.setProfitLossQuantity(new BigDecimal("5.00"));
        line.setActualQuantity(new BigDecimal("100.00"));
        line.setProfitLossAmount(new BigDecimal("125.50"));
        line.setPrice(new BigDecimal("25.10"));
        line.setInventoryItemId("inv_001");
        line.setSupplierId("supplier_001");
        line.setTraceNo("TRACE001");
        line.setRemake("盘点正常");
        line.setOccurrenceTime(LocalDateTime.of(2026, 7, 18, 10, 0, 0));

        vo.setLines(List.of(line));
        return vo;
    }

    // ============================================================
    // 提交审批
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/submitInboundReceiptApproval")
    @Operation(
            summary = "提交审批",
            description = "将草稿（DRAFT）状态的进货单提交至审批流程，状态变更为待审批（PENDING_APPROVAL）。可指定审批人（approverId）。服务端写入 applicantId 和 applyTime。"
    )
    public JsonVO<InboundApprovalResultVO> submitInboundReceiptApproval(
            @Parameter(description = "提交审批入参", required = true)
            @RequestBody @Valid SubmitInboundReceiptApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "APPROVAL"));
    }

    // ============================================================
    // 撤回审批
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/withdrawInboundReceiptApproval")
    @Operation(
            summary = "撤回审批",
            description = "将待审批（PENDING_APPROVAL）状态的进货单撤回到草稿（DRAFT）状态。仅单据提交人可执行此操作。"
    )
    public JsonVO<InboundApprovalResultVO> withdrawInboundReceiptApproval(
            @Parameter(description = "撤回审批入参", required = true)
            @RequestBody @Valid WithdrawInboundReceiptApprovalDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "WITHDRAW"));
    }

    // ============================================================
    // 审批通过
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/agreeInboundReceipt")
    @Operation(
            summary = "审批通过",
            description = "审批人通过进货单，状态变更为已通过（AGREE）。服务端写入 approverId 和 approvalTime。如需上传医保，可传入 contractBusNo。"
    )
    public JsonVO<InboundApprovalResultVO> agreeInboundReceipt(
            @Parameter(description = "审批通过入参", required = true)
            @RequestBody @Valid AgreeInboundReceiptDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "AGREE"));
    }

    // ============================================================
    // 驳回审批
    // ============================================================
    @Override
    @PostMapping("/purchase-inbound/rejectInboundReceipt")
    @Operation(
            summary = "驳回审批",
            description = "审批人驳回进货单，状态变更为已驳回（REJECT）。必须填写驳回原因（rejectReason）。服务端写入 approverId 和 approvalTime。"
    )
    public JsonVO<InboundApprovalResultVO> rejectInboundReceipt(
            @Parameter(description = "驳回审批入参", required = true)
            @RequestBody @Valid RejectInboundReceiptDTO dto) {
        return JsonVO.success(new InboundApprovalResultVO(true, "REJECT"));
    }

    @Operation(summary = "删除出库单")
    @DeleteMapping("/removeStockOut")
    @Override
    public JsonVO<String> removeStockOut(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "获取药库出库单详情")
    @GetMapping("/getStockOutDetail")
    @Override
    public JsonVO<StockOutDetailVO> getStockOutDetail(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "获取药库出库单列表")
    @GetMapping("/getPharmacy")
    @Override
    public JsonVO<PageDTO<StockOutListVO>> getPharmacy(@RequestBody StockOutQuery stockOutQuery) {
        return null;
    }


    @PostMapping("/pharmacy/stock-out/submit-approval")
    @Operation(summary = "提交出库审批")
    @Override
    public JsonVO<String> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("提交成功，当前状态：APPROVAL，审批人ID：" + dto.getApproverId());
    }

    @PostMapping("/pharmacy/stock-out/withdraw-approval")
    @Operation(summary = "撤回出库审批")
    @Override
    public JsonVO<String> withdrawStockOutApproval(@Validated @RequestBody WithdrawStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("撤回成功，当前状态：WITHDRAW");
    }

    @PostMapping("/pharmacy/stock-out/agree")
    @Operation(summary = "通过出库审批")
    @Override
    public JsonVO<String> agreeStockOut(@Validated @RequestBody AgreeStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("审批通过，当前状态：AGREE，药房入库壳单号：PI202607200001");
    }

    @PostMapping("/pharmacy/stock-out/reject")
    @Operation(summary = "驳回出库审批")
    @Override
    public JsonVO<String> rejectStockOut(@Validated @RequestBody RejectStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("驳回成功，当前状态：REJECT");
    }
}
