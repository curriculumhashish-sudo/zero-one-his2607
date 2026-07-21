package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.*;
import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptDeleteDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.*;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.query.j6.pharmacy.StockOutQuery;
import org.springframework.web.bind.annotation.*;
import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

import com.zeroone.star.project.query.j6.pharmacy.InventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.InboundReceiptQuery;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.validation.annotation.Validated;
import lombok.RequiredArgsConstructor;
import io.swagger.v3.oas.annotations.Parameter;


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

    /**
     * 获取库存明细列表(分页+条件)
     */
    @PostMapping("/pageInventoryDetailCurrent")
    @Operation(
            summary = "分页条件查询库存明细",
            description = "携带筛选条件分页查询库存明细数据",
            responses = @ApiResponse(responseCode = "200", description = "查询成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<InventoryDetailVO> pageInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO) {
        return JsonVO.fail("接口未完成");
    }

    /**
     * 停供/取消停供
     */
    @PostMapping("/resumeInventoryItems")
    @Operation(
            summary = "批量停供/取消停供商品",
            description = "批量修改商品停供状态，返回受影响行数",
            responses = @ApiResponse(responseCode = "200", description = "操作成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<Integer> updateStatus(@RequestBody InventoryStatusDTO inventoryStatusDTO) {
        return JsonVO.fail("接口未完成");
    }

    /**
     * 导出库存明细Excel
     */
    @PostMapping("/exportInventoryDetailCurrent")
    @Operation(
            summary = "按条件导出库存明细Excel",
            description = "根据筛选条件生成Excel文件，返回base64文件信息",
            responses = @ApiResponse(responseCode = "200", description = "导出成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<ExcelExportVO> exportInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO) {
        return JsonVO.fail("接口未完成");
    }

    @Override
    @Operation(summary = "获取进货单列表")
    public JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query) {
        // TODO 调用 Service 查询数据库，返回列表
        return null;
    }

    @Override
    @Operation(summary = "获取进货单详情")
    public JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query) {
        // TODO 调用 Service 查详情，返回头+明细行
        return null;
    }

    @Override
    @Operation(summary = "删除进货单")
    public JsonVO<String> removeInboundReceipt(@Valid InboundReceiptDeleteDTO dto) {
        // TODO 调用 Service 执行删除
        return null;
    }

    @Override
    @Operation(summary = "新增药库进货单", description = "新增一张药库进货单草稿，后续数据落库由业务服务实现。")
    @PostMapping
    public JsonVO<InboundReceiptSaveResultVO> addInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "DRAFT", "新增成功", null, dto.getVersion()));
    }

    @Override
    @Operation(summary = "保存药库进货单", description = "保存已存在的药库进货单草稿，后续数据落库由业务服务实现。")
    @PutMapping
    public JsonVO<InboundReceiptSaveResultVO> saveInboundReceipt(@Valid @RequestBody InboundReceiptSaveDTO dto) {
        return JsonVO.success(buildResult(dto.getBusNo(), "DRAFT", "保存成功", null, dto.getVersion()));
    }

    @Override
    @Operation(summary = "保存并提交药库进货单审批", description = "保存单据并提交审批，提交后进入待审批状态，后续数据落库和审批流由业务服务实现。")
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
}
