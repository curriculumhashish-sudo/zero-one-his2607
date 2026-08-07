package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.*;
import com.zeroone.star.project.j6.pharmacy.WarehouseStocktakeApis;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDeleteVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/stocktake")
@RequiredArgsConstructor
@Tag(name = "药库盘点单", description = "药库管理相关接口")
public class WarehouseStocktakeController implements WarehouseStocktakeApis {

    private final IWkfSupplyRequestService IwkfSupplyRequestService;

    private final IStocktakeService stocktakeService;

    @Override
    @GetMapping("list")
    @Operation(summary = "获取单据列表（条件+分页）")
    public JsonVO<PharmacyInventoryListVO> listInventory(PharmacyInventoryQuery query) {
        PharmacyInventoryListVO pageResult = IwkfSupplyRequestService.listInventory(query);
        return JsonVO.success(pageResult);
    }

    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取单据详情")
    public JsonVO<PharmacyInventoryDetailVO> getInventoryDetail(PharmacyInventoryDetailQuery query) {
        PharmacyInventoryDetailVO vo = IwkfSupplyRequestService.getStocktakeDetail(query);
        return JsonVO.success(vo);
    }

    @Override
    @DeleteMapping("/{busNo}")
    @Operation(summary = "删除单据")
    public JsonVO<Boolean> deleteStocktake(@PathVariable String busNo) {
        PharmacyInventoryDeleteVO vo = IwkfSupplyRequestService.deleteStocktake(busNo);
        return JsonVO.success(vo.getSuccess());
    }

    @Override
    @PostMapping("/create")
    @Operation(summary = "新增单据", description = "发送简洁上下文（库房、盘点人等），返回空白草稿的完整文档结构。")
    public JsonVO<StocktakeDetailVO> createStocktake(@RequestBody StocktakeAddDTO dto) {
        try {
            return JsonVO.success(stocktakeService.createStocktake(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PutMapping("/{busNo}/save")
    @Operation(summary = "保存单据", description = "填入实盘数据 + 快照锁定账面数（请求结构同新增返回），状态=草稿")
    public JsonVO<StocktakeDetailVO> saveStocktake(@PathVariable String busNo, @RequestBody StocktakeSaveRequestDTO dto) {
        try {
            return JsonVO.success(stocktakeService.saveStocktake(busNo, dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @PutMapping("/{busNo}/submit")
    @Operation(summary = "保存并提交审批", description = "填入实盘数据 + 快照锁定账面数（请求结构同新增返回），状态草稿→待审批")
    public JsonVO<StocktakeDetailVO> submitStocktake(@PathVariable String busNo, @RequestBody StocktakeSaveRequestDTO dto) {
        try {
            return JsonVO.success(stocktakeService.submitStocktake(busNo, dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Resource
    private IStocktakeApprovalService stocktakeApprovalService;

    @Override
    @PostMapping("/submit-approval")
    @Operation(summary = "提交审批")
    public JsonVO<Boolean> submitStocktakeApproval(@Valid @RequestBody StocktakeSubmitApprovalDTO dto) {
        return execute(stocktakeApprovalService::submit, dto);
    }

    @Override
    @PostMapping("/withdraw-approval")
    @Operation(summary = "撤回审批")
    public JsonVO<Boolean> withdrawStocktakeApproval(@Valid @RequestBody StocktakeWithdrawApprovalDTO dto) {
        return execute(stocktakeApprovalService::withdraw, dto);
    }

    @Override
    @PostMapping("/approve")
    @Operation(summary = "审批通过")
    public JsonVO<Boolean> agreeStocktakeApproval(@Valid @RequestBody StocktakeAgreeApprovalDTO dto) {
        return execute(stocktakeApprovalService::agree, dto);
    }

    @Override
    @PostMapping("/reject")
    @Operation(summary = "驳回审批")
    public JsonVO<Boolean> rejectStocktakeApproval(@Valid @RequestBody StocktakeRejectApprovalDTO dto) {
        return execute(stocktakeApprovalService::reject, dto);
    }

    @FunctionalInterface
    private interface ApprovalAction<T> {
        Boolean execute(T dto);
    }

    private <T> JsonVO<Boolean> execute(ApprovalAction<T> action, T dto) {
        try {
            return JsonVO.success(action.execute(dto));
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
