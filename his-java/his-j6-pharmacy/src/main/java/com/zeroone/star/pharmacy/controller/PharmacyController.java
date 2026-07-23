package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeWithdrawApprovalDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/warehouse/stocktake")
@Tag(name = "Warehouse stocktake approval", description = "Warehouse stocktake approval flow")
public class PharmacyController implements PharmacyApis {
    @Resource
    private IStocktakeApprovalService stocktakeApprovalService;

    @Override
    @Operation(summary = "Submit warehouse stocktake approval")
    @PostMapping("/submit-approval")
    public JsonVO<Boolean> submitStocktakeApproval(@Valid @RequestBody StocktakeSubmitApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.submit(dto));
    }

    @Override
    @Operation(summary = "Withdraw warehouse stocktake approval")
    @PostMapping("/withdraw-approval")
    public JsonVO<Boolean> withdrawStocktakeApproval(@Valid @RequestBody StocktakeWithdrawApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.withdraw(dto));
    }

    @Override
    @Operation(summary = "Approve warehouse stocktake")
    @PostMapping("/approve")
    public JsonVO<Boolean> agreeStocktakeApproval(@Valid @RequestBody StocktakeAgreeApprovalDTO dto) {
        return execute(() -> stocktakeApprovalService.agree(dto));
    }

    @Override
    @Operation(summary = "Reject warehouse stocktake")
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
