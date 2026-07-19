package com.zeroone.star.j3.department.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentUpdateDTO;
import com.zeroone.star.project.j3.department.DepartmentApis;
import com.zeroone.star.project.query.j3.department.DepartmentQuery;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：科室管理传输层
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@RestController
@RequestMapping("/database/departments")
@Slf4j
@Tag(name = "科室管理")
public class DepartmentController implements DepartmentApis {

    @GetMapping("/tree")
    @Operation(summary = "获取科室名称树（支持门诊/住院筛选)")
    @Override
    public JsonVO<DepartmentDTO> getDepartmentTree(@RequestBody DepartmentTreeQuery query) {
        return null;
    }

    @GetMapping("/{id}")
    @Operation(summary = "获取科室详情")
    @Override
    public JsonVO<DepartmentDTO> getDepartmentDetail(@PathVariable Long id) {
        return null;
    }

    @PostMapping
    @Operation(summary = "新增科室")
    @Override
    public JsonVO<String> createDepartment(@RequestBody DepartmentCreateDTO dto) {
        return null;
    }

    @PutMapping("/batch-status")
    @Operation(summary = "启用/停用科室(支持批量)")
    @Override
    public JsonVO<String> activeOrInactiveDepartment(@RequestBody List<Long> ids) {
        return null;
    }

    @GetMapping("/page")
    @Operation(summary = "获取科室列表（条件+分页）")
    @Override
    public JsonVO<PageDTO<DepartmentDTO>> getDepartmentPage(@Valid DepartmentQuery query) {
        return null;
    }

    @PutMapping("/{id}")
    @Operation(summary = "修改科室")
    @Override
    public JsonVO<String> updateDepartment(
            @Parameter(description = "科室ID", example = "322663105141149696", required = true)
            @PathVariable String id,
            @Valid @RequestBody DepartmentUpdateDTO dto) {
        return null;
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "删除科室")
    @Override
    public JsonVO<String> deleteDepartment(
            @Parameter(description = "科室ID", example = "322663105141149696", required = true)
            @PathVariable String id) {
        return null;
    }
}
