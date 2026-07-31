package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.service.IOrderSetService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.j5.configuration.orderset.OrderSetApis;
import com.zeroone.star.project.query.j5.configuration.orderset.OrderSetQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetDetailVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetListVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetNameVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 配置管理-组套模板控制器。
 */
@RestController
@Validated
@RequiredArgsConstructor
@RequestMapping("/order-set")
@Tag(name = "配置管理-组套模板", description = "组套模板增删改查接口")
public class OrderSetController implements OrderSetApis {

    private final IOrderSetService orderSetService;

    @Override
    @GetMapping("/names")
    @Operation(summary = "获取组套名称", description = "获取所有启用的组套名称列表")
    public JsonVO<List<OrderSetNameVO>> listOrderSetNames() {
        return JsonVO.success(orderSetService.listOrderSetNames());
    }

    @Override
    @PostMapping("/list")
    @Operation(summary = "获取组套列表", description = "条件+分页查询组套模板")
    public JsonVO<PageDTO<OrderSetListVO>> listOrderSets(@RequestBody OrderSetQuery query) {
        return JsonVO.success(orderSetService.listOrderSets(query));
    }

    @Override
    @GetMapping("/{id}")
    @Operation(summary = "获取组套详情", description = "根据ID获取组套模板详情")
    public JsonVO<OrderSetDetailVO> getOrderSetDetail(
            @Parameter(description = "组套模板ID", required = true)
            @PathVariable("id") String id) {
        return JsonVO.success(orderSetService.getOrderSetDetail(id));
    }

    @Override
    @PostMapping
    @Operation(summary = "新增组套", description = "先保存聚合；存在有效明细时自动发布，空明细时保留草稿")
    public JsonVO<OrderSetOperationVO> addOrderSet(@RequestBody OrderSetAddDTO dto) {
        return JsonVO.success(orderSetService.addOrderSet(dto));
    }

    @Override
    @PutMapping
    @Operation(summary = "修改组套", description = "按请求中的完整明细集合同步新增、修改和软删除子项")
    public JsonVO<OrderSetOperationVO> modifyOrderSet(@RequestBody OrderSetModifyDTO dto) {
        return JsonVO.success(orderSetService.modifyOrderSet(dto));
    }

    @Override
    @DeleteMapping("/{id}")
    @Operation(summary = "删除组套", description = "软删除组套及其全部明细")
    public JsonVO<String> removeOrderSet(
            @Parameter(description = "组套模板编号", required = true)
            @PathVariable String id) {
        return JsonVO.success(orderSetService.removeOrderSet(id));
    }

    @Override
    @PutMapping("/status")
    @Operation(summary = "启/停用组套", description = "DRAFT可发布为ACTIVE；ACTIVE可停用为RETIRED；RETIRED不可重新发布")
    public JsonVO<OrderSetOperationVO> modifyOrderSetStatus(@RequestBody OrderSetStatusDTO dto) {
        return JsonVO.success(orderSetService.modifyOrderSetStatus(dto));
    }
}
