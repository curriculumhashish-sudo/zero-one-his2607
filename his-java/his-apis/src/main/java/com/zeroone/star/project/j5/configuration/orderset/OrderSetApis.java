package com.zeroone.star.project.j5.configuration.orderset;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetAddDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetModifyDTO;
import com.zeroone.star.project.dto.j5.configuration.orderset.OrderSetStatusDTO;
import com.zeroone.star.project.query.j5.configuration.orderset.OrderSetQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetDetailVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetListVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetNameVO;
import com.zeroone.star.project.vo.j5.configuration.orderset.OrderSetOperationVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 配置管理-组套模板API
 */
@Tag(name = "配置管理-组套模板", description = "组套模板增删改查接口")
@RequestMapping("/order-set")
public interface OrderSetApis {

    /**
     * 获取组套名称列表（下拉选项）
     */
    @Operation(summary = "获取组套名称", description = "获取所有启用的组套名称列表")
    @GetMapping("/names")
    JsonVO<List<OrderSetNameVO>> listOrderSetNames();

    /**
     * 分页查询组套模板列表
     */
    @Operation(summary = "获取组套列表", description = "条件+分页查询组套模板")
    @PostMapping("/list")
    JsonVO<PageDTO<OrderSetListVO>> listOrderSets(@RequestBody OrderSetQuery query);

    /**
     * 根据ID获取组套模板详情
     */
    @Operation(summary = "获取组套详情", description = "根据ID获取组套模板详情")
    @GetMapping("/{id}")
    JsonVO<OrderSetDetailVO> getOrderSetDetail(
            @Parameter(description = "组套模板ID", required = true)
            @PathVariable("id") String id);

    /**
     * 新增组套模板。
     * @param dto 新增数据
     * @return 操作结果
     */
    @Operation(summary = "新增组套", description = "新增组套模板")
    @PostMapping
    JsonVO<OrderSetOperationVO> addOrderSet(@Valid @RequestBody OrderSetAddDTO dto);

    /**
     * 修改组套模板。
     * @param dto 修改数据
     * @return 操作结果
     */
    @Operation(summary = "修改组套", description = "修改组套模板")
    @PutMapping
    JsonVO<OrderSetOperationVO> modifyOrderSet(@Valid @RequestBody OrderSetModifyDTO dto);

    /**
     * 删除组套模板。
     * @param id 组套模板编号
     * @return 被删除的组套模板编号
     */
    @Operation(summary = "删除组套", description = "删除组套模板")
    @DeleteMapping("/{id}")
    JsonVO<String> removeOrderSet(
            @Parameter(description = "组套模板ID", required = true)
            @NotBlank(message = "组套模板编号不能为空")
            @PathVariable("id") String id);

    /**
     * 启用或停用组套模板。
     * @param dto 状态操作数据
     * @return 操作结果
     */
    @Operation(summary = "启/停用组套", description = "启用或停用组套模板")
    @PutMapping("/status")
    JsonVO<OrderSetOperationVO> modifyOrderSetStatus(@Valid @RequestBody OrderSetStatusDTO dto);
}
