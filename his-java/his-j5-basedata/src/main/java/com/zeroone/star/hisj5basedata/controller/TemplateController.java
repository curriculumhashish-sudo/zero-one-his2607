package com.zeroone.star.hisj5basedata.controller;

import com.zeroone.star.hisj5basedata.query.OrderTemplateNameQuery;
import com.zeroone.star.hisj5basedata.query.OrderTemplateQuery;
import com.zeroone.star.hisj5basedata.service.IOrderTemplateService;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateDetailVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateListVO;
import com.zeroone.star.hisj5basedata.vo.OrderTemplateNameVO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 组套模板控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@RestController
@RequestMapping("/template")
@Validated
@Tag(name = "order-template", description = "组套模板接口")
public class TemplateController {

    @Resource
    private IOrderTemplateService orderTemplateService;

    @Operation(summary = "获取组套名称列表")
    @GetMapping("/names")
    public JsonVO<List<OrderTemplateNameVO>> listNames(@Valid OrderTemplateNameQuery query) {
        List<OrderTemplateNameVO> result = orderTemplateService.listNames(query);
        return JsonVO.success(result);
    }

    @Operation(summary = "获取组套列表（分页）")
    @GetMapping("/list")
    public JsonVO<PageDTO<OrderTemplateListVO>> list(@Valid OrderTemplateQuery query) {
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
}