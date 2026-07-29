package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.j7.pharmacy.PharmacyStockTakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 盘点单审批 前端控制器
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
@RestController
@RequestMapping("/j7")
@Tag(name = "盘点单审批")
@Validated
public class WkfSupplyRequestController implements PharmacyStockTakeApis {

    @Resource
    private IWkfSupplyRequestService wkfSupplyRequestService;

    @PutMapping("/submit")
    @Operation(summary = "提交审批")
    @Override
    public JsonVO<StocktakeStatusVO> submit(@RequestBody OperatorDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.submit(dto));
    }

    @PutMapping("/approve")
    @Operation(summary = "审批通过")
    @Override
    public JsonVO<StocktakeStatusVO> approve(@RequestBody ApproveDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.approve(dto));
    }

    @PutMapping("/reject")
    @Operation(summary = "审批驳回")
    @Override
    public JsonVO<StocktakeStatusVO> reject(@RequestBody ApproveDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.reject(dto));
    }

    @PutMapping("/withdraw")
    @Operation(summary = "撤回审批")
    @Override
    public JsonVO<StocktakeStatusVO> withdraw(@RequestBody OperatorDTO dto) {
        return JsonVO.success(wkfSupplyRequestService.withdraw(dto));
    }

}
