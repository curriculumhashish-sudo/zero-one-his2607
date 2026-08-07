package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStockOutService;
import com.zeroone.star.pharmacy.service.IStockoutApprovalService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.*;
import com.zeroone.star.project.j6.pharmacy.WarehouseStockOutApis;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/stockout")
@RequiredArgsConstructor
@Tag(name = "药库出库单", description = "药库管理相关接口")
public class WarehouseStockOutController implements WarehouseStockOutApis {

    @Resource
    private IStockOutService stockOutService;

    @Resource
    private IStockoutApprovalService stockoutApprovalService;

    @Override
    @PostMapping("/add")
    @Operation(summary = "新增单据", description = "发送简洁上下文（源药库、目的药房等），返回空白草稿的完整文档结构（空卡片+空明细）。")
    public JsonVO<StockOutDetailVO> addStockOut(@Valid @RequestBody StockOutAddDTO dto) {
        try {
            return JsonVO.success(stockOutService.addStockOut(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PostMapping("/save")
    @Operation(summary = "保存单据", description = "保存已存在的药库出库单草稿（请求结构同新增返回），校验版本后重建单据明细。")
    public JsonVO<StockOutDetailVO> saveStockOut(@Valid @RequestBody StockOutSaveRequestDTO dto) {
        try {
            return JsonVO.success(stockOutService.saveStockOut(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PostMapping("/save-and-submit")
    @Operation(summary = "保存并提交审批", description = "保存单据并提交审批（请求结构同新增返回），提交后进入待审批状态。")
    public JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(@Valid @RequestBody StockOutSaveRequestDTO dto) {
        try {
            return JsonVO.success(stockOutService.saveAndSubmitApproval(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @DeleteMapping("/{busNo}/delete")
    @Operation(summary = "删除单据")
    public JsonVO<String> removeStockOut(@PathVariable String busNo) {
        String result = stockOutService.deleteDraftByBusNo(busNo);
        if ("success".equals(result)) {
            return JsonVO.success("删除成功");
        } else {
            return JsonVO.fail(result);
        }
    }

    @Override
    @GetMapping("/{busNo}")
    @Operation(summary = "获取单据详情")
    public JsonVO<StockOutDetailVO> getStockOutDetail(@PathVariable String busNo) {
        StockOutDetailVO detail = stockOutService.getDetailByBusNo(busNo);
        return detail != null ? JsonVO.success(detail) : JsonVO.fail("单据不存在");
    }

    @Override
    @GetMapping("/getPharmacy")
    @Operation(summary = "获取单据列表（条件+分页）")
    public JsonVO<PageDTO<StockOutListVO>> getPharmacy(@Valid StockOutQuery stockOutQuery) {
        PageDTO<StockOutListVO> pageDTO = stockOutService.getPharmacyList(stockOutQuery);
        return JsonVO.success(pageDTO);
    }

    @Override
    @PostMapping("/submit-approval")
    @Operation(summary = "提交审批")
    public JsonVO<StockOutApprovalResultVO> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        Boolean submitted = stockoutApprovalService.submitStockOutApproval(dto);
        if (submitted) {
            return JsonVO.success(new StockOutApprovalResultVO(true, "APPROVAL"));
        } else {
            return JsonVO.fail("提交审批失败");
        }
    }

    @Override
    @PostMapping("/withdraw-approval")
    @Operation(summary = "撤回审批")
    public JsonVO<StockOutApprovalResultVO> withdrawStockOutApproval(@Validated @RequestBody WithdrawStockOutApprovalDTO dto) {
        Boolean withdrawn = stockoutApprovalService.withdrawStockOutApproval(dto);
        if (withdrawn) {
            return JsonVO.success(new StockOutApprovalResultVO(true, "WITHDRAW"));
        } else {
            return JsonVO.fail("撤回审批失败");
        }
    }

    @Override
    @PostMapping("/agree")
    @Operation(summary = "审批通过")
    public JsonVO<StockOutApprovalResultVO> agreeStockOut(@Validated @RequestBody AgreeStockOutDTO dto) {
        Boolean agreed = stockoutApprovalService.agreeStockOut(dto);
        if (agreed) {
            return JsonVO.success(new StockOutApprovalResultVO(true, "AGREE"));
        } else {
            return JsonVO.fail("审批通过失败");
        }
    }

    @Override
    @PostMapping("/reject")
    @Operation(summary = "驳回审批")
    public JsonVO<StockOutApprovalResultVO> rejectStockOut(@Validated @RequestBody RejectStockOutDTO dto) {
        Boolean rejected = stockoutApprovalService.rejectStockOut(dto);
        if (rejected) {
            return JsonVO.success(new StockOutApprovalResultVO(true, "REJECT"));
        } else {
            return JsonVO.fail("驳回审批失败");
        }
    }
}
