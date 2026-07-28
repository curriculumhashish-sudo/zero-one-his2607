package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.stocktake.StocktakeApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：盘点单查询提交接口
 * </p>
 * <p>
 * 功能说明：基于 {@code wkf_supply_request} 表（{@code type_enum = INVENTORY_CHECK}）
 * 提供盘点单的三大 RESTful 接口：
 * 1. G9-01 获取盘点单列表：分页查询，按 bus_no 聚合，支持状态/库房/时间筛选
 * 2. G9-02 获取盘点单详情：根据盘点单号查询汇总信息及明细列表
 * 3. G9-03 提交盘点单审批：草稿 → 已提交（待审批），批量更新同一 bus_no 下所有行
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@RestController
@RequestMapping("/stocktake")
@Tag(name = "盘点单管理", description = "盘点单查询提交接口")
public class StocktakeController implements StocktakeApis {

    @Resource
    private IStocktakeService stocktakeService;

    /**
     * G9-01 获取盘点单列表（分页查询）
     * @param query 查询条件
     * @return 分页数据
     */
    @Operation(summary = "获取盘点单列表", description = "分页查询盘点单列表，按 bus_no 聚合，支持状态/库房/时间筛选")
    @GetMapping("/list")
    @Override
    public JsonVO<PageDTO<StocktakeListVO>> queryStocktakeList(StocktakeQuery query) {
        PageDTO<StocktakeListVO> pageDTO = stocktakeService.queryStocktakeList(query);
        return JsonVO.success(pageDTO);
    }

    /**
     * G9-02 获取盘点单详情（含明细列表）
     * @param id 盘点单号（bus_no）
     * @return 盘点单详情
     */
    @Operation(summary = "获取盘点单详情", description = "根据盘点单号查询汇总信息及明细列表")
    @GetMapping("/detail/{id}")
    @Override
    public JsonVO<StocktakeDetailVO> queryStocktakeDetail(
            @Parameter(description = "盘点单号", example = "PD20260720001")
            @PathVariable String id) {
        StocktakeDetailVO detailVO = stocktakeService.queryStocktakeDetail(id);
        if (detailVO == null) {
            return JsonVO.fail("盘点单不存在：" + id);
        }
        return JsonVO.success(detailVO);
    }

    /**
     * G9-03 提交盘点单审批
     * <p>
     * 业务校验：单据必须存在且为盘点单类型、状态为草稿、至少有一条明细行。
     * 状态变更：DRAFT → SUBMITTED。
     * </p>
     * @param dto 提交审批参数
     * @return 提交结果
     */
    @Operation(summary = "提交盘点单审批", description = "将草稿状态的盘点单提交至审批流程，状态变更为已提交")
    @PostMapping("/submit")
    @Override
    public JsonVO<String> submitStocktake(@RequestBody @Valid StocktakeSubmitDTO dto) {
        try {
            String result = stocktakeService.submitStocktake(dto);
            return JsonVO.success(result);
        } catch (IllegalArgumentException e) {
            // 业务校验失败，返回失败状态码和具体原因
            return JsonVO.fail(e.getMessage());
        }
    }
}
