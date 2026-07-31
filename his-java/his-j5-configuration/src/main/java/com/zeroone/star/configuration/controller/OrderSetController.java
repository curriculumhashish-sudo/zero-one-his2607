package com.zeroone.star.configuration.controller;


import com.zeroone.star.configuration.service.IOrderSetService;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.j5.configuration.orderset.OrderSetApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import lombok.RequiredArgsConstructor;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * 配置管理-组套模板写操作控制器。
 */
@RestController
@Validated
@RequiredArgsConstructor
@RequestMapping("/order-set")
@Tag(name = "配置管理-组套模板", description = "组套模板新增、修改、删除和发布/停用")
public class OrderSetController implements OrderSetApis {

    private final IOrderSetService orderSetService;

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
            @Parameter(description = "组套模板编号", example = "329888224518344704", required = true)
            @PathVariable String id) {
        return JsonVO.success(orderSetService.removeOrderSet(id));
    }

    @Override
    @PutMapping("/status")
    @Operation(summary = "发布或停用组套", description = "DRAFT可发布为ACTIVE；ACTIVE可停用为RETIRED；RETIRED不可重新发布")
    public JsonVO<OrderSetOperationVO> modifyOrderSetStatus(@RequestBody OrderSetStatusDTO dto) {
        return JsonVO.success(orderSetService.modifyOrderSetStatus(dto));
    }
}
