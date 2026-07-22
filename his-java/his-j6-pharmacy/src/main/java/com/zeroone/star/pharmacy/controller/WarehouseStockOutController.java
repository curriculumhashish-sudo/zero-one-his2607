package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.WithdrawStockOutApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.WarehouseStockOutApis;
import com.zeroone.star.project.query.j6.pharmacy.stockout.StockOutQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/stockout")
@RequiredArgsConstructor
@Tag(name = "药库出库单", description = "药库管理相关接口")
public class WarehouseStockOutController implements WarehouseStockOutApis {
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
    public JsonVO<String> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("提交成功，当前状态：APPROVAL，审批人ID：" + dto.getApproverId());
    }

    @PostMapping("/withdraw-approval")
    @Operation(summary = "撤回审批")
    @Override
    public JsonVO<String> withdrawStockOutApproval(@Validated @RequestBody WithdrawStockOutApprovalDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("撤回成功，当前状态：WITHDRAW");
    }

    @PostMapping("/agree")
    @Operation(summary = "审批通过")
    @Override
    public JsonVO<String> agreeStockOut(@Validated @RequestBody AgreeStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("审批通过，当前状态：AGREE，药房入库壳单号：PI202607200001");
    }

    @PostMapping("/reject")
    @Operation(summary = "驳回审批")
    @Override
    public JsonVO<String> rejectStockOut(@Validated @RequestBody RejectStockOutDTO dto) {
        // TODO: 桩实现，待后续对接Service层
        return JsonVO.success("驳回成功，当前状态：REJECT");
    }
}
