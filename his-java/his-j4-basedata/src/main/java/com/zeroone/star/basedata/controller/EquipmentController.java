package com.zeroone.star.basedata.controller;

import com.zeroone.star.basedata.service.IEquipmentService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentEditDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentStatusDTO;
import com.zeroone.star.project.dto.j4.equipment.EquipmentUpdateDTO;
import com.zeroone.star.project.j4.basedata.EquipmentApis;
import com.zeroone.star.project.query.j4.equipment.EquipmentPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVo;
import com.zeroone.star.project.vo.j4.equipment.EquipmentUpdateVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：器材管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j4组
 * @version 1.0.0
 */
@Validated
@RestController
@RequestMapping("/equipment")
@Tag(name = "basedata-equipment", description = "器材管理")
public class EquipmentController implements EquipmentApis {
    @Resource
    private IEquipmentService equipmentService;

    @Override
    @GetMapping("/query-equipment-list")
    @Operation(summary = "分页查询器材列表", description = "分页查询器材列表")
    public JsonVO<PageDTO<EquipmentVO>> queryPage(@Valid EquipmentPageQuery query) {
        return null;
    }

    @Override
    @PostMapping("/add-equipment")
    @Operation(summary = "添加器材", description = "添加器材")
    public JsonVO<String> add(@RequestBody @Valid EquipmentAddDTO dto) {
        return null;
    }

    @Override
    @PostMapping("/modify-equipment")
    @Operation(summary = "编辑器材", description = "编辑器材")
    public JsonVO<EquipmentUpdateVO> edit(@RequestBody @Valid EquipmentEditDTO dto) {
        return null;
    }

    @Override
    @PostMapping("/update-equipment")
    @Operation(summary = "更新器材", description = "更新器材")
    public JsonVO<EquipmentUpdateVO> update(@RequestBody @Valid EquipmentUpdateDTO dto) {
        return null;
    }

    @Override
    @PutMapping("/modify-equipment-status")
    @Operation(summary = "切换器材状态", description = "切换器材状态")
    public JsonVO<EquipmentUpdateVO> toggleStatus(@RequestBody @Valid EquipmentStatusDTO dto) {
        return null;
    }

    @Override
    @GetMapping("/get-equipment-detail")
    @Operation(summary = "获取器材详情", description = "获取器材详情")
    public JsonVO<EquipmentVO> getDetail(@RequestParam @NotBlank String id) {
        return null;
    }

    @Override
    @GetMapping("/get-equipment-categories")
    @Operation(summary = "获取器材分类", description = "获取器材分类")
    public JsonVO<List<EquipmentCategoryVo>> getCategories() {
        return null;
    }

    @Override
    @DeleteMapping("/delete-equipment")
    @Operation(summary = "删除器材", description = "删除器材")
    public JsonVO<String> delete(@RequestParam @NotBlank String id) {
        return null;
    }
}
