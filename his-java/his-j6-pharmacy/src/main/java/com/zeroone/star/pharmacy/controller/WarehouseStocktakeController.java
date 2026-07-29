package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.*;
import com.zeroone.star.project.j6.pharmacy.WarehouseStocktakeApis;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDeleteVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.PharmacyInventoryListVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.PharmacyInventoryVO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

@RestController
@RequestMapping("/stocktake")
@RequiredArgsConstructor
@Tag(name = "药库盘点单", description = "药库管理相关接口")
public class WarehouseStocktakeController implements WarehouseStocktakeApis {

    @Resource
    private final IWkfSupplyRequestService IwkfSupplyRequestService;

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
        return JsonVO.success(new PharmacyInventoryDetailVO());
    }


    @Override
    @DeleteMapping("/{busNo}")
    @Operation(summary = "删除单据")
    public JsonVO<Boolean> deleteStocktake(@PathVariable String busNo) {
        PharmacyInventoryDeleteVO vo=IwkfSupplyRequestService.deleteStocktake(busNo);
        return JsonVO.success(vo.getSuccess());
    }

    @Override
    @Operation(summary = "新增单据", description = "生成空盘点单, 状态=草稿")
    @PostMapping("/create")
    public JsonVO<StocktakeDetailVO> createStocktake(@RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(buildExampleDetailVO());
    }

    @Override
    @Operation(summary = "保存单据", description = "填入实盘数据 + 快照锁定账面数，状态=草稿")
    @PutMapping("/{busNo}/save")
    public JsonVO<StocktakeDetailVO> saveStocktake(@PathVariable String busNo, @RequestBody StocktakeSaveDTO dto) {
        StocktakeDetailVO vo = buildExampleDetailVO();
        vo.setBusNo(busNo);
        return JsonVO.success(vo);
    }

    @Override
    @Operation(summary = "保存并提交审批", description = "填入实盘数据，状态草稿→待审批")
    @PutMapping("/{busNo}/submit")
    public JsonVO<StocktakeDetailVO> submitStocktake(@PathVariable String busNo,@RequestBody StocktakeSaveDTO dto) {
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
