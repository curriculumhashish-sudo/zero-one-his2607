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
        PharmacyInventoryListVO pageResult=IwkfSupplyRequestService.listInventory(query);
        return JsonVO.success(pageResult);
    }

    @Override
    @GetMapping("/detail")
    @Operation(summary = "获取单据详情")
    public JsonVO<PharmacyInventoryDetailVO> getInventoryDetail(PharmacyInventoryDetailQuery query) {
        PharmacyInventoryDetailVO vo=IwkfSupplyRequestService.getStocktakeDetail(query);
        return JsonVO.success(vo);
    }

    @Override
    @DeleteMapping("/{busNo}")
    @Operation(summary = "删除单据")
    public JsonVO<Boolean> deleteStocktake(@PathVariable String busNo) {
        PharmacyInventoryDeleteVO vo=IwkfSupplyRequestService.deleteStocktake(busNo);
        return JsonVO.success(vo.getSuccess());
    }

    /**
     * 新增盘点单
     * <p>生成空盘点单头，不生成明细行（明细由前端逐行添加后通过 saveStocktake 保存）</p>
     */
    @Override
    @Operation(summary = "新增单据", description = "生成空盘点单, 状态=草稿")
    @PostMapping("/create")
    public JsonVO<StocktakeDetailVO> createStocktake(@RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.createStocktake(dto));
    }

    /**
     * 保存盘点单
     * <p>先删后插策略：快照锁定账面数 + 从库存项补全物品信息 + 批量写入明细行</p>
     */
    @Override
    @Operation(summary = "保存单据", description = "填入实盘数据 + 快照锁定账面数，状态=草稿")
    @PutMapping("/{busNo}/save")
    public JsonVO<StocktakeDetailVO> saveStocktake(@PathVariable String busNo, @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveStocktake(busNo, dto));
    }

    /**
     * 保存盘点单并提交审批
     * <p>保存盘点数据 + 状态流转 DRAFT → PENDING_APPROVAL</p>
     */
    @Override
    @Operation(summary = "保存并提交审批", description = "填入实盘数据，状态草稿→待审批")
    @PutMapping("/{busNo}/submit")
    public JsonVO<StocktakeDetailVO> submitStocktake(@PathVariable String busNo, @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.submitStocktake(busNo, dto));
    }

    @Resource
    private IStocktakeApprovalService stocktakeApprovalService;

    @Override
    @Operation(summary = "提交审批")
    @PostMapping("/submit-approval")
    public JsonVO<Boolean> submitStocktakeApproval(@Valid @RequestBody StocktakeSubmitApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.submit(dto));
    }

    @Override
    @Operation(summary = "撤回审批")
    @PostMapping("/withdraw-approval")
    public JsonVO<Boolean> withdrawStocktakeApproval(@Valid @RequestBody StocktakeWithdrawApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.withdraw(dto));
    }

    @Override
    @Operation(summary = "审批通过")
    @PostMapping("/approve")
    public JsonVO<Boolean> agreeStocktakeApproval(@Valid @RequestBody StocktakeAgreeApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.agree(dto));
    }

    @Override
    @Operation(summary = "驳回审批")
    @PostMapping("/reject")
    public JsonVO<Boolean> rejectStocktakeApproval(@Valid @RequestBody StocktakeRejectApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.reject(dto));
    }

    private JsonVO<Boolean> execute(ApprovalAction action) {
        try {
            return JsonVO.success(action.execute());
        } catch (Exception e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @FunctionalInterface
    private interface ApprovalAction {
        Boolean execute();
    }
}
