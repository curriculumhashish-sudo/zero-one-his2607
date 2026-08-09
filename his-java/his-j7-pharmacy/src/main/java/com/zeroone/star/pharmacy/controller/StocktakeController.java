package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.stocktake.StocktakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.*;

import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：盘点单新增保存与查询控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@RestController
@Validated
@Tag(name = "盘点单管理", description = "盘点单新增保存、查询和提交审批")
public class StocktakeController implements StocktakeApis {

    @Resource
    private IStocktakeService stocktakeService;

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeSaveJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST20260730100000001\",\"busNo\":\"PST20260730100000001\",\"statusEnum\":\"DRAFT\",\"createdByName\":null,\"createdAt\":\"2026-07-30 10:00:00\",\"approverName\":null,\"approvalTime\":null,\"warehouseName\":null,\"totalPrice\":0,\"items\":[]}}")))
    })
    public JsonVO<StocktakeSaveVO> addStocktake() {
        return JsonVO.success(stocktakeService.addStocktake());
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeListPageJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"pageIndex\":1,\"pageSize\":10,\"total\":1,\"pages\":1,\"rows\":[{\"id\":\"PD20260701001\",\"busNo\":\"PD20260701001\",\"statusEnum\":\"APPROVED\",\"createdByName\":\"张三\",\"createdAt\":\"2026-07-01 09:00:00\",\"approverName\":\"李四\",\"approvalTime\":\"2026-07-01 14:00:00\",\"warehouseName\":\"门诊药房\",\"profitLossPrice\":1250.50}]}}")))
    })
    public JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(@RequestBody @Valid StocktakeQuery query) {
        return JsonVO.success(stocktakeService.queryStocktakeList(query));
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeDetailJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"329874660147597312\",\"busNo\":\"PD2026-07-010001\"}}")))
    })
    public JsonVO<StocktakeDetailVO> queryStocktakeDetail(@PathVariable("id") String id) {
        return JsonVO.success(stocktakeService.queryStocktakeDetail(id));
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeSaveJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST20260730100000001\",\"busNo\":\"PST20260730100000001\",\"statusEnum\":\"DRAFT\",\"createdByName\":\"张三\",\"createdAt\":\"2026-07-30 10:00:00\",\"approverName\":null,\"approvalTime\":null,\"warehouseName\":\"门诊药房\",\"totalPrice\":1187.50,\"items\":[{\"id\":\"1\",\"itemName\":\"阿莫西林胶囊\",\"specification\":\"0.25g*24粒\",\"manufacturer\":\"哈药集团\",\"lotNumber\":\"20260101\",\"bookQuantity\":100,\"totalQuantity\":95,\"itemQuantity\":-5,\"price\":12.50,\"actualAmount\":1187.50,\"profitLossPrice\":-62.50,\"remark\":\"正常损耗\"}]}}")))
    })
    public JsonVO<StocktakeSaveVO> saveStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveStocktake(dto));
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(schema = @Schema(implementation = StocktakeSaveJsonVO.class,
                example = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST20260730100000001\",\"busNo\":\"PST20260730100000001\",\"statusEnum\":\"PENDING_APPROVAL\",\"createdByName\":\"张三\",\"createdAt\":\"2026-07-30 10:00:00\",\"approverName\":null,\"approvalTime\":null,\"warehouseName\":\"门诊药房\",\"totalPrice\":1187.50,\"items\":[{\"id\":\"1\",\"itemName\":\"阿莫西林胶囊\",\"specification\":\"0.25g*24粒\",\"manufacturer\":\"哈药集团\",\"lotNumber\":\"20260101\",\"bookQuantity\":100,\"totalQuantity\":95,\"itemQuantity\":-5,\"price\":12.50,\"actualAmount\":1187.50,\"profitLossPrice\":-62.50,\"remark\":\"正常损耗\"}]}}")))
    })
    public JsonVO<StocktakeSaveVO> saveAndSubmitStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveAndSubmitStocktake(dto));
    }
}
