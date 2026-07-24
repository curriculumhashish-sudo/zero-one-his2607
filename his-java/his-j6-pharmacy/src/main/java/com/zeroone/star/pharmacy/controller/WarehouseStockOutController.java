package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutSaveResultVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.WithdrawStockOutApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.WarehouseStockOutApis;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutApprovalResultVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
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
public class WarehouseStockOutController implements WarehouseStockOutApis {
    @PostMapping("/save")
    @Operation(summary = "保存出库单")
    @Override
    public JsonVO<StockOutSaveResultVO> saveStockOut(@Valid @RequestBody StockOutSaveDTO dto) {
        // 最简单的实现：直接返回成功
        return JsonVO.success(new StockOutSaveResultVO());
    }
    @Operation(summary = "删除单据")
    @DeleteMapping("/removeStockOut")
    @Override
    public JsonVO<String> removeStockOut(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "获取单据详情")
    @GetMapping("/getStockOutDetail")
    @Override
    public JsonVO<StockOutDetailVO> getStockOutDetail(@PathVariable String id) {
        return null;
    }

    @Operation(summary = "获取单据列表（条件+分页）")
    @GetMapping("/getPharmacy")
    @Override
    public JsonVO<PageDTO<StockOutListVO>> getPharmacy(@RequestBody StockOutQuery stockOutQuery) {
        return null;
    }


    @PostMapping("/submit-approval")
    @Operation(summary = "提交审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success(new StockOutApprovalResultVO(true, "APPROVAL"));
    }

    @PostMapping("/withdraw-approval")
    @Operation(summary = "撤回审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> withdrawStockOutApproval(@Validated @RequestBody WithdrawStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success(new StockOutApprovalResultVO(true, "WITHDRAW"));
    }

    @PostMapping("/agree")
    @Operation(summary = "审批通过")
    @Override
    public JsonVO<StockOutApprovalResultVO> agreeStockOut(@Validated @RequestBody AgreeStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success(new StockOutApprovalResultVO(true, "AGREE"));
    }

    @PostMapping("/reject")
    @Operation(summary = "驳回审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> rejectStockOut(@Validated @RequestBody RejectStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success(new StockOutApprovalResultVO(true, "REJECT"));
    }
}
