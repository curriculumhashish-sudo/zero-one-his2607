package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.stocktake.StocktakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveVO;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
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
    public JsonVO<StocktakeSaveVO> addStocktake() {
        return JsonVO.success(stocktakeService.addStocktake());
    }

    @Override
    public JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(@Valid @ModelAttribute StocktakeQuery query) {
        return JsonVO.success(stocktakeService.queryStocktakeList(query));
    }

    @Override
    public JsonVO<StocktakeDetailVO> queryStocktakeDetail(@PathVariable("id") String id) {
        return JsonVO.success(stocktakeService.queryStocktakeDetail(id));
    }

    @Override
    public JsonVO<StocktakeSaveVO> saveStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveStocktake(dto));
    }

    @Override
    public JsonVO<StocktakeSaveVO> saveAndSubmitStocktake(@Valid @RequestBody StocktakeSaveDTO dto) {
        return JsonVO.success(stocktakeService.saveAndSubmitStocktake(dto));
    }

    @Override
    public JsonVO<String> submitStocktake(@Valid @RequestBody StocktakeSubmitDTO dto) {
        return JsonVO.success(stocktakeService.submitStocktake(dto));
    }
}
