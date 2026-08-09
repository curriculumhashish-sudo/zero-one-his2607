package com.zeroone.star.pharmacy.controller.end;

import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.dto.j7.pharmacy.StocktakeDeleteDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.ApproveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.StocktakeStatusVO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteJsonVO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/j7/stocktake-approval")
@Tag(name = "药房盘点单", description = "药房盘点单基础流程")
public class yaofangpandiandan {

    @Resource
    private IWkfSupplyRequestService wkfSupplyRequestService;

    @Resource
    private IStocktakeService stocktakeService;

    @PutMapping("/submit")
    @Operation(summary = "提交审批")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeStatusJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"status\":\"PENDING_APPROVAL\"}}")))
    })
    public JsonVO<StocktakeStatusVO> submit(@Valid @RequestBody OperatorDTO dto) {
        try {
            return JsonVO.success(wkfSupplyRequestService.submit(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @PutMapping("/approve")
    @Operation(summary = "审批通过")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeStatusJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"status\":\"APPROVED\"}}")))
    })
    public JsonVO<StocktakeStatusVO> approve(@Valid @RequestBody ApproveDTO dto) {
        try {
            return JsonVO.success(wkfSupplyRequestService.approve(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @PutMapping("/reject")
    @Operation(summary = "审批驳回")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeStatusJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"status\":\"REJECTED\"}}")))
    })
    public JsonVO<StocktakeStatusVO> reject(@Valid @RequestBody ApproveDTO dto) {
        try {
            return JsonVO.success(wkfSupplyRequestService.reject(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @PutMapping("/withdraw")
    @Operation(summary = "撤回审批")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeStatusJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"status\":\"WITHDRAWN\"}}")))
    })
    public JsonVO<StocktakeStatusVO> withdraw(@Valid @RequestBody OperatorDTO dto) {
        try {
            return JsonVO.success(wkfSupplyRequestService.withdraw(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @DeleteMapping("/delete")
    @Operation(summary = "删除盘点单据", description = "软删除指定单据号的盘点单，仅草稿/待审核状态可删，同时解锁关联的库存锁定记录")
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = SupplyDocumentDeleteJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"deleted\":true}}")))
    })
    public JsonVO<SupplyDocumentDeleteVO> deleteStocktake(@Valid @RequestBody StocktakeDeleteDTO dto) {
        try {
            return JsonVO.success(stocktakeService.deleteStocktake(dto));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }
}
