package com.zeroone.star.j3.department.controller;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentBatchDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentDTO;
import com.zeroone.star.project.dto.j3.department.DepartmentUpdateDTO;
import com.zeroone.star.project.j3.department.DepartmentApis;
import com.zeroone.star.project.query.j3.department.DepartmentQuery;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.department.DepartmentVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.*;

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
@RequestMapping("/departments")
@Slf4j
@Tag(name = "科室管理")
public class DepartmentController implements DepartmentApis {

    @GetMapping("/tree")
    @Operation(summary = "获取科室名称树（支持门诊/住院筛选)")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "数据不存在")
    })
    @Override
    public JsonVO<DepartmentDTO> getDepartmentTree(DepartmentTreeQuery query) {
        return null;
    }

    @GetMapping("/{id}")
    @Operation(summary = "获取科室详情")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    @Override
    public JsonVO<DepartmentDTO> getDepartmentDetail(@PathVariable Long id) {
        return null;
    }

    @PostMapping
    @Operation(summary = "新增科室")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "新增成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<String> createDepartment(@Valid @RequestBody DepartmentCreateDTO dto) {
        return null;
    }

    @PutMapping("/batch-status")
    @Operation(summary = "启用/停用科室(支持批量)")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "更新成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    @Override
    public JsonVO<String> activeOrInactiveDepartment(@Valid @RequestBody DepartmentBatchDTO dto) {
        return null;
    }

    @GetMapping("/page")
    @Operation(summary = "获取科室列表（条件+分页）",
            description = "支持按关键词、状态、类型和类别筛选科室，返回分页结果")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误")
    })
    @Override
    public JsonVO<PageDTO<DepartmentVO>> getDepartmentPage(@Valid DepartmentQuery query) {
        return null;
    }

    @PutMapping("/{id}")
    @Operation(summary = "修改科室", description = "根据科室ID修改科室信息")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "修改成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    @Override
    public JsonVO<String> updateDepartment(
            @Parameter(description = "科室ID", example = "322663105141149696", required = true)
            @PathVariable("id") String id,
            @Valid @RequestBody DepartmentUpdateDTO dto) {
        return null;
    }

    @DeleteMapping("/{id}")
    @Operation(summary = "删除科室",
            description = "根据科室ID删除科室，业务实现阶段需校验子科室及业务引用")
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "删除成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    @Override
    public JsonVO<String> deleteDepartment(
            @Parameter(description = "科室ID", example = "322663105141149696", required = true)
            @PathVariable("id") String id) {
        return null;
    }
}
