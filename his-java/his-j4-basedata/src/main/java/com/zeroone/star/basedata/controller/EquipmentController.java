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
@Tag(name = "器材管理", description = "器材目录的增删改查及分类查询")
public class EquipmentController implements EquipmentApis {
    @Resource
    private IEquipmentService equipmentService;

    /**
     * 分页查询器材列表
     *
     * @param query 分页查询条件（支持关键字搜索、分类筛选）
     * @return 分页器材数据
     */
    @Override
    @GetMapping("/query-equipment-list")
    @Operation(summary = "分页查询器材列表", description = "分页查询器材列表")
    public JsonVO<PageDTO<EquipmentVO>> queryPage(@Valid EquipmentPageQuery query) {
        return null;
    }

    /**
     * 添加器材
     *
     * @param equipmentAddDTO 器材详情信息DTO
     * @return 无返回值
     */
    @Override
    @PostMapping("/add-equipment")
    @Operation(summary = "新增器材", description = "新增器材")
    public JsonVO<Void> addEquipment(@RequestBody @Valid EquipmentAddDTO equipmentAddDTO) {
        return null;
    }

    /**
     * 编辑器材（携带编辑校验码，支持并发编辑保护）
     *
     * @param dto 编辑器材数据（包含主键及编辑业务码）
     * @return 编辑确认结果
     */
    @Override
    @PostMapping("/modify-equipment")
    @Operation(summary = "编辑器材", description = "编辑器材")
    public JsonVO<EquipmentUpdateVO> edit(@RequestBody @Valid EquipmentEditDTO dto) {
        return null;
    }

    /**
     * 修改器材（无编辑校验码，直接覆盖更新）
     *
     * @param dto 修改器材数据
     * @return 修改确认结果
     */
    @Override
    @PostMapping("/update-equipment")
    @Operation(summary = "更新器材", description = "更新器材")
    public JsonVO<EquipmentUpdateVO> update(@RequestBody @Valid EquipmentUpdateDTO dto) {
        return null;
    }

    /**
     * 切换器材状态（启用/禁用取反）
     *
     * @param dto 切换状态请求（仅主键）
     * @return 切换后的器材状态信息
     */
    @Override
    @PutMapping("/modify-equipment-status")
    @Operation(summary = "切换器材状态", description = "切换器材状态")
    public JsonVO<EquipmentUpdateVO> toggleStatus(@RequestBody @Valid EquipmentStatusDTO dto) {
        return null;
    }

    /**
     * 获取器材详情
     *
     * @param equipmentId 器材id
     * @return 器材详情
     */
    @Override
    @GetMapping("/get-by-id")
    @Operation(summary = "查询器材详情", description = "查询器材详情")
    public JsonVO<EquipmentVO> getEquipmentByEquipmentId(@RequestParam @Valid Long equipmentId) {
        return null;
    }

    /**
     * 获取选择器材种类列表
     *
     * @return 可选择的器材种类列表
     */
    @Override
    @GetMapping("/query-equipment-category")
    @Operation(summary = "查询器材分类列表", description = "查询器材分类列表")
    public JsonVO<List<EquipmentCategoryVo>> queryEquipmentCategoryList() {
        return null;
    }

    /**
     * 批量删除器材信息
     *
     * @param equipmentIds 器材id集合
     * @return 无返回值
     */
    @Override
    @DeleteMapping("/remove-by-ids")
    @Operation(summary = "批量删除器材", description = "批量删除器材")
    public JsonVO<Void> removeEquipmentByEquipmentIds(@RequestBody @Valid List<Long> equipmentIds) {
        return null;
    }
}
