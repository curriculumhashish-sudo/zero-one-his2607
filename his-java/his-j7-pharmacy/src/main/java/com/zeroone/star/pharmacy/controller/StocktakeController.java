package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.stocktake.StocktakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailJsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import com.zeroone.star.project.vo.stocktake.StocktakePageJsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveJsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveVO;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.ExampleObject;
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
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StocktakeSaveJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST202608080001\",\"busNo\":\"PST202608080001\",\"statusEnum\":\"DRAFT\",\"createdAt\":\"2026-08-08 09:00:00\",\"warehouseId\":\"322954949549363200\",\"warehouseName\":\"西药房\",\"stocktakePharmacy\":\"西药房\",\"itemCount\":0,\"totalPrice\":0,\"operateTime\":\"2026-08-08 09:00:00\",\"items\":[]}}")))
    })
    public JsonVO<StocktakeSaveVO> addStocktake() {
        return JsonVO.success(stocktakeService.addStocktake());
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StocktakePageJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"pageIndex\":1,\"pageSize\":10,\"total\":1,\"pages\":1,\"rows\":[{\"id\":\"330268165805182976\",\"busNo\":\"PST202608080001\",\"statusEnum\":\"DRAFT\",\"createdBy\":\"308791351298756608\",\"createdByName\":\"张三\",\"createdAt\":\"2026-08-08 09:00:00\",\"warehouseId\":\"322954949549363200\",\"warehouseName\":\"西药房\",\"stocktakePharmacy\":\"西药房\",\"totalPrice\":1187.50,\"profitLossPrice\":-25.00,\"applicantId\":\"308791351298756608\",\"applicantName\":\"张三\",\"applyTime\":\"2026-08-08 09:30:00\",\"approverId\":\"308791351298756609\",\"approverName\":\"李四\",\"approvalTime\":\"2026-08-08 10:00:00\"}]}}")))
    })
    public JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(@RequestBody @Valid StocktakeQuery query) {
        return JsonVO.success(stocktakeService.queryStocktakeList(query));
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StocktakeDetailJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST202608080001\",\"busNo\":\"PST202608080001\",\"statusEnum\":\"DRAFT\",\"statusDesc\":\"草稿\",\"warehouseId\":\"322954949549363200\",\"warehouseName\":\"西药房\",\"stocktakePharmacy\":\"西药房\",\"stocktakeType\":\"PHARMACY\",\"totalPrice\":1187.50,\"totalProfitPrice\":0,\"totalLossPrice\":25.00,\"profitLossPrice\":-25.00,\"itemCount\":1,\"createdBy\":\"308791351298756608\",\"createdByName\":\"张三\",\"applicantId\":\"308791351298756608\",\"applicantName\":\"张三\",\"applyTime\":\"2026-08-08 09:30:00\",\"approverId\":\"308791351298756609\",\"approverName\":\"李四\",\"approvalTime\":\"2026-08-08 10:00:00\",\"remark\":\"月度盘点\",\"createdAt\":\"2026-08-08 09:00:00\",\"items\":[{\"id\":\"1\",\"itemId\":\"322998385762111488\",\"itemName\":\"阿莫西林胶囊\",\"specification\":\"0.25g*24粒\",\"manufacturer\":\"广州白云山制药\",\"lotNumber\":\"001-009\",\"bookQuantity\":100,\"totalQuantity\":98,\"itemQuantity\":-2,\"unitCode\":\"box\",\"unitName\":\"box\",\"price\":12.50,\"totalPrice\":1225.00,\"profitLossPrice\":-25.00,\"remark\":\"正常损耗\"}]}}")))
    })
    public JsonVO<StocktakeDetailVO> queryStocktakeDetail(@PathVariable("id") String id) {
        try {
            return JsonVO.success(stocktakeService.queryStocktakeDetail(id));
        } catch (IllegalArgumentException e) {
            return JsonVO.fail(e.getMessage());
        }
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StocktakeSaveJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST202608080001\",\"busNo\":\"PST202608080001\",\"statusEnum\":\"DRAFT\",\"warehouseId\":\"322954949549363200\",\"warehouseName\":\"西药房\",\"stocktakePharmacy\":\"西药房\",\"itemCount\":1,\"totalPrice\":1225.00,\"operateTime\":\"2026-08-08 09:00:00\",\"items\":[{\"id\":\"1\",\"itemName\":\"阿莫西林胶囊\",\"specification\":\"0.25g*24粒\",\"manufacturer\":\"广州白云山制药\",\"lotNumber\":\"001-009\",\"bookQuantity\":100,\"totalQuantity\":98,\"itemQuantity\":-2,\"price\":12.50,\"totalPrice\":1225.00,\"profitLossPrice\":-25.00,\"remark\":\"正常损耗\"}]}}")))
    })
    public JsonVO<StocktakeSaveVO> saveStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveStocktake(dto));
    }

    @Override
    @ApiResponses({
        @ApiResponse(responseCode = "200", description = "操作成功",
            content = @Content(mediaType = "application/json",
                schema = @Schema(implementation = StocktakeSaveJsonVO.class),
                examples = @ExampleObject(value = "{\"code\":10000,\"message\":\"操作成功\",\"data\":{\"id\":\"PST202608080001\",\"busNo\":\"PST202608080001\",\"statusEnum\":\"PENDING_APPROVAL\",\"warehouseId\":\"322954949549363200\",\"warehouseName\":\"西药房\",\"stocktakePharmacy\":\"西药房\",\"itemCount\":1,\"totalPrice\":1225.00,\"operateTime\":\"2026-08-08 09:00:00\",\"items\":[{\"id\":\"1\",\"itemName\":\"阿莫西林胶囊\",\"specification\":\"0.25g*24粒\",\"manufacturer\":\"广州白云山制药\",\"lotNumber\":\"001-009\",\"bookQuantity\":100,\"totalQuantity\":98,\"itemQuantity\":-2,\"price\":12.50,\"totalPrice\":1225.00,\"profitLossPrice\":-25.00,\"remark\":\"正常损耗\"}]}}")))
    })
    public JsonVO<StocktakeSaveVO> saveAndSubmitStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveAndSubmitStocktake(dto));
    }
}
