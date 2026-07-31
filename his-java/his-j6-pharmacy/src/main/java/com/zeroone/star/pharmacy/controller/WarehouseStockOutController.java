package com.zeroone.star.pharmacy.controller;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.pharmacy.service.IStockOutService;
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
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/stockout")
@RequiredArgsConstructor
@Tag(name = "药库出库单", description = "药库管理相关接口")
@Valid
public class WarehouseStockOutController implements WarehouseStockOutApis {

    @Resource
    private IStockOutService stockOutService;

    @PostMapping("/save-and-submit")
    @Operation(summary = "保存并提交审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> saveAndSubmitApproval(@Valid @RequestBody StockOutSaveDTO dto) {
        return stockOutService.saveAndSubmitApproval(dto);
    }

    @PostMapping("/add")
    @Operation(summary = "新增单据")
    @Override
    public JsonVO<StockOutSaveResultVO> addStockOut(@Valid @RequestBody StockOutSaveDTO dto) {
        // 新增时 busNo 为空，系统生成
        dto.setBusNo(null);
        return stockOutService.saveStockOut(dto);
    }


    @PostMapping("/save")
    @Operation(summary = "保存单据")
    @Override
    public JsonVO<StockOutSaveResultVO> saveStockOut(@Valid @RequestBody StockOutSaveDTO dto) {
        // 调用 Service
        return stockOutService.saveStockOut(dto);
    }

    @PostMapping("/submit-approval")
    @Operation(summary = "提交审批")
    @Override
    public JsonVO<StockOutApprovalResultVO> submitStockOutApproval(@Validated @RequestBody SubmitStockOutApprovalDTO dto) {
        // 调用 Service
        return stockOutService.submitApproval(dto);
    }
    @Operation(summary = "删除单据")
    @DeleteMapping("/{busNo}/delete")
    @Override
    public JsonVO<String> removeStockOut(@PathVariable String busNo) {
        // 根据 ID 查询单据是否存在
        //boolean success = stockOutService.deleteDraftByBusNo(busNo);
        //return success ? JsonVO.success("删除成功") : JsonVO.fail(success ? "删除成功" : "单据不存在或状态非草稿");
        return null;
    }

    @Operation(summary = "获取单据详情")
    @GetMapping("/{busNo}")
    @Override
    public JsonVO<StockOutDetailVO> getStockOutDetail(@PathVariable String busNo) {
        //StockOutDetailVO detail = stockOutService.getDetailByBusNo(busNo);
        //return detail != null ? JsonVO.success(detail) : JsonVO.fail("单据不存在");
        return null;
    }

    @Operation(summary = "获取单据列表（条件+分页）")
    @GetMapping("/getPharmacy")
    @Override
    public JsonVO<PageDTO<StockOutListVO>> getPharmacy(@Valid StockOutQuery stockOutQuery) {
        //PageDTO<StockOutListVO> pageDTO = stockOutService.getPharmacyList(stockOutQuery);
        //return JsonVO.success(pageDTO);
        return null;
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
