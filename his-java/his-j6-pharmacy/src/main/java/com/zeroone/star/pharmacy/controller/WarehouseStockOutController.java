package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStockoutApprovalService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.*;
import com.zeroone.star.project.j6.pharmacy.WarehouseStockOutApis;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutSaveResultVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/stockout")
@RequiredArgsConstructor
@Tag(name = "药库出库单", description = "药库管理相关接口")
@Valid
public class WarehouseStockOutController implements WarehouseStockOutApis {
    @Override
    public JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(StockOutSaveDTO dto) {
        return null;
    }

    @Override
    public JsonVO<StockOutSaveResultVO> addStockOut(StockOutSaveDTO dto) {
        return null;
    }

    @Override
    public JsonVO<StockOutSaveResultVO> saveStockOut(StockOutSaveDTO dto) {
        return null;
    }

    @Operation(summary = "删除单据")
    @DeleteMapping("/removeStockOut/{id}")
    @Override
    public JsonVO<String> removeStockOut(@PathVariable String id) {
        return JsonVO.fail("功能开发中");
    }

    @Operation(summary = "获取单据详情")
    @GetMapping("/getStockOutDetail/{id}")
    @Override
    public JsonVO<StockOutDetailVO> getStockOutDetail(@PathVariable String id) {
        return JsonVO.fail("功能开发中");
    }

    @Operation(summary = "获取单据列表（条件+分页）")
    @GetMapping("/getPharmacy")
    @Override
    public JsonVO<PageDTO<StockOutListVO>> getPharmacy(@Valid StockOutQuery stockOutQuery) {
        return JsonVO.fail("功能开发中");
    }


    private final IStockoutApprovalService stockoutApprovalService;

    @PostMapping("/submit-approval")
    @Operation(summary = "提交审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        Boolean submitted = stockoutApprovalService.submitStockOutApproval(dto);
        if(submitted){
            return JsonVO.success(new StockOutApprovalResultVO(true, "APPROVAL"));
        }else{
            return JsonVO.fail("提交审批失败");
        }

    }

    @PostMapping("/withdraw-approval")
    @Operation(summary = "撤回审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> withdrawStockOutApproval(@Validated @RequestBody WithdrawStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        Boolean withdrawn = stockoutApprovalService.withdrawStockOutApproval(dto);
        if(withdrawn){
            return JsonVO.success(new StockOutApprovalResultVO(true, "WITHDRAW"));
        }else{
            return JsonVO.fail("撤回审批失败");
        }
    }

    @PostMapping("/agree")
    @Operation(summary = "审批通过")
    @Override
    public JsonVO<StockOutApprovalResultVO> agreeStockOut(@Validated @RequestBody AgreeStockOutDTO dto) {
        Boolean agreed = stockoutApprovalService.agreeStockOut(dto);
        if (agreed) {
            return JsonVO.success(new StockOutApprovalResultVO(true, "AGREE"));
        } else {
            return JsonVO.fail("审批通过失败");
        }
    }

    @PostMapping("/reject")
    @Operation(summary = "驳回审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> rejectStockOut(@Validated @RequestBody RejectStockOutDTO dto) {
        Boolean rejected = stockoutApprovalService.rejectStockOut(dto);
        if(rejected){
            return JsonVO.success(new StockOutApprovalResultVO(true, "REJECT"));
        }else{
            return JsonVO.fail("驳回审批失败");
        }
    }
}
