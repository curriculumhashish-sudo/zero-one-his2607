package com.zeroone.star.basedata.controller;

import com.zeroone.star.project.dto.j4.equipment.EquipmentAddDTO;
import com.zeroone.star.project.j4.equipment.EquipmentApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.equipment.EquipmentCategoryVo;
import com.zeroone.star.project.vo.j4.equipment.EquipmentVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：器材管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/equipment")
@Tag(name = "器材管理", description = "器材目录的增删改查及分类查询")
public class EquipmentController implements EquipmentApis {

    /**
     * 获取器材详情
     * @param equipmentId 器材id
     * @return 器材详情
     */
    @Override
    @GetMapping
    @Operation(summary = "查询器材详情")
    public JsonVO<EquipmentVO> getEquipmentByEquipmentId(Long equipmentId) {
        return null;
    }

    /**
     * 获取选择器材种类列表
     * @return 可选择的器材种类列表
     */
    @Override
    @GetMapping("/category")
    @Operation(summary = "查询器材分类列表")
    public JsonVO<List<EquipmentCategoryVo>> queryEquipmentCategoryList() {
        return null;
    }

    /**
     * 批量删除器材信息
     * @param equipmentIds 器材id集合
     * @return 无返回值
     */
    @Override
    @DeleteMapping
    @Operation(summary = "批量删除器材")
    public JsonVO<Void> removeEquipmentByEquipmentIds(List<Long> equipmentIds) {
        return null;
    }

    /**
     * 添加器材
     * @param equipmentAddDTO 器材详情信息DTO
     * @return 无返回值
     */
    @Override
    @PostMapping
    @Operation(summary = "新增器材")
    public JsonVO<Void> addEquipment(@RequestBody EquipmentAddDTO equipmentAddDTO) {
        return null;
    }
}
