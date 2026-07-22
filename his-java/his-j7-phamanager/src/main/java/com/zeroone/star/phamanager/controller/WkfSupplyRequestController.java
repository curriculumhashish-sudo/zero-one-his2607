package com.zeroone.star.phamanager.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.OperatorDTO;
import com.zeroone.star.project.dto.j7.pharmacy.PharmacyStockTakeDTO;
import com.zeroone.star.project.j7.PharmacyStockTakeApis;
import com.zeroone.star.project.query.StockTakeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.StocktakeStatusVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

/**
 * <p>
 * 工作流 supply request 前端控制器
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
@RestController
@RequestMapping("/phamanager")
@Tag(name = "审批状态流转")
@Validated
public class WkfSupplyRequestController implements PharmacyStockTakeApis {

    @GetMapping("/list")
    @Operation(summary = "获取库存盘点列表")
    @Override
    public JsonVO<PageDTO<PharmacyStockTakeDTO>> getPharmacyStockTakeList(@RequestBody StockTakeQuery query) {
        return null;
    }

    @PutMapping("/approve")
    @Operation(summary = "审批库存盘点")
    @Override
    public JsonVO<StocktakeStatusVO> approvePharmacyStockTake(@RequestBody ApproveDTO dto) {
        return null;
    }

    @PutMapping("/reject")
    @Operation(summary = "拒绝库存盘点")
    public JsonVO<StocktakeStatusVO> rejectPharmacyStockTake(@RequestBody ApproveDTO dto) {
        return null;
    }

    @Operation(summary = "撤销库存盘点")
    @PutMapping("/withdraw")
    public JsonVO<StocktakeStatusVO> withdrawPharmacyStockTake(@RequestBody OperatorDTO dto) {
        return null;
    }
}
