package com.zeroone.star.configuration.controller;

import com.zeroone.star.configuration.entity.OrderTemplate;
import com.zeroone.star.configuration.query.OrderTemplateNameQuery;
import com.zeroone.star.configuration.query.OrderTemplateQuery;
import com.zeroone.star.configuration.service.IOrderTemplateService;
import com.zeroone.star.configuration.vo.OrderTemplateDetailVO;
import com.zeroone.star.configuration.vo.OrderTemplateListVO;
import com.zeroone.star.configuration.vo.OrderTemplateNameVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/config/order-template")
@Tag(name = "配置管理-组套模板", description = "组套模板接口")
public class OrderTemplateController {

    @Resource
    private IOrderTemplateService orderTemplateService;

    @Operation(summary = "获取组套名称列表")
    @GetMapping("/names")
    public JsonVO<List<OrderTemplateNameVO>> listNames(OrderTemplateNameQuery query) {
        List<OrderTemplateNameVO> result = orderTemplateService.listNames(query);
        return JsonVO.success(result);
    }

    @Operation(summary = "获取组套列表（分页）")
    @GetMapping("/list")
    public JsonVO<PageDTO<OrderTemplateListVO>> list(OrderTemplateQuery query) {
        return JsonVO.success(PageDTO.create(orderTemplateService.list(query)));
    }

    @Operation(summary = "获取组套详情（含明细行）")
    @GetMapping("/{id}/detail")
    public JsonVO<OrderTemplateDetailVO> detail(
            @Parameter(description = "组套模板 ID", required = true)
            @PathVariable String id) {
        OrderTemplateDetailVO detail = orderTemplateService.getDetail(id);
        if (detail == null) {
            return JsonVO.create(null, ResultStatus.FAIL.getCode(), "组套模板不存在");
        }
        return JsonVO.success(detail);
    }

    @Operation(summary = "新增组套", description = "创建新的组套模板")
    @PostMapping("/add")
    public JsonVO<Void> add(@RequestBody OrderTemplate orderTemplate) {
        return JsonVO.success(null);
    }

    @Operation(summary = "修改组套", description = "更新组套模板信息")
    @PutMapping("/update")
    public JsonVO<Void> update(@RequestBody OrderTemplate orderTemplate) {
        return JsonVO.success(null);
    }

    @Operation(summary = "删除组套", description = "删除指定的组套模板")
    @DeleteMapping("/delete/{id}")
    public JsonVO<Void> delete(
            @Parameter(description = "组套模板ID", required = true)
            @PathVariable String id) {
        return JsonVO.success(null);
    }

    @Operation(summary = "启/停用组套", description = "启用或停用组套模板")
    @PutMapping("/status/{id}")
    public JsonVO<Void> status(
            @Parameter(description = "组套模板ID", required = true)
            @PathVariable String id,
            @Parameter(description = "状态（ENABLED/DISABLED）", required = true)
            @RequestParam String statusEnum) {
        return JsonVO.success(null);
    }
}
