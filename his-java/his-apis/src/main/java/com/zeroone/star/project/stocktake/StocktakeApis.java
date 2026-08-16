package com.zeroone.star.project.stocktake;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.PathVariable;

/**
 * <p>
 * 描述：盘点单查询提交接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Tag(name = "盘点单管理", description = "盘点单新增保存、查询和提交审批")
@RequestMapping("/api/inventory-check")
public interface StocktakeApis {

    /**
     * 新增盘点单草稿
     *
     * @return 草稿信息
     */
    @Operation(summary = "新增盘点单")
    @GetMapping("/new")
    JsonVO<StocktakeSaveVO> addStocktake();

    /**
     * 获取盘点单列表（分页查询）
     * @param query 查询条件
     * @return 盘点单分页列表
     */
    @Operation(summary = "获取盘点单列表")
    @PostMapping("/list")
    JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(
            @RequestBody @Valid StocktakeQuery query
    );

    /**
     * 获取盘点单详情
     * @param id 盘点单ID
     * @return 盘点单详情
     */
    @Operation(summary = "获取盘点单详情")
    @GetMapping("/detail/{id}")
    JsonVO<StocktakeDetailVO> queryStocktakeDetail(
            @Parameter(description = "盘点单号", required = true)
            @PathVariable("id")
            String id
    );

    /**
     * 保存盘点单
     *
     * @param dto 保存参数
     * @return 保存结果
     */
    @Operation(summary = "保存盘点单")
    @PostMapping("/save")
    JsonVO<StocktakeSaveVO> saveStocktake(
            @Parameter(description = "盘点单保存参数", required = true)
            @Valid @RequestBody StocktakeSaveDTO dto
    );

    /**
     * 保存并提交盘点单审批
     *
     * @param dto 保存参数
     * @return 保存结果
     */
    @Operation(summary = "保存并提交审批")
    @PostMapping("/save-submit")
    JsonVO<StocktakeSaveVO> saveAndSubmitStocktake(
            @Parameter(description = "盘点单保存参数", required = true)
            @Valid @RequestBody StocktakeSaveDTO dto
    );

}
