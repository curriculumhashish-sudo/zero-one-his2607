package com.zeroone.star.j3.department.controller;

import com.zeroone.star.project.dto.j3.department.DepartmentCreateDTO;
import com.zeroone.star.project.j3.department.DepartmentApis;
import com.zeroone.star.project.query.j3.department.DepartmentTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.department.DepartmentDetailVO;
import com.zeroone.star.project.vo.j3.department.DepartmentTreeVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import io.swagger.v3.oas.annotations.tags.Tag;
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
@RequestMapping("/departments")
@Slf4j
@Tag(name = "科室管理")
// TODO: 后期需调用Service层获得真实数据
public class DepartmentController implements DepartmentApis {

    @PostMapping("/tree")
    @Operation(summary = "获取科室名称树（支持门诊/住院筛选)")
    @Override
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    public JsonVO<DepartmentTreeVO> getDepartmentTree(@RequestBody DepartmentTreeQuery query) {
        return JsonVO.success(null);
    }

    @GetMapping("/{id}")
    @Operation(summary = "获取科室详情")
    @Override
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    public JsonVO<DepartmentDetailVO> getDepartmentDetail(@PathVariable Long id) {
        return JsonVO.success(null);
    }

    @PostMapping
    @Operation(summary = "新增科室")
    @Override
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    public JsonVO<String> createDepartment(@RequestBody DepartmentCreateDTO dto) {
        return JsonVO.success(null);
    }

    @PutMapping("/batch-status")
    @Operation(summary = "启用/停用科室(支持批量)")
    @Override
    @ApiResponses({
            @ApiResponse(responseCode = "200", description = "成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class))),
            @ApiResponse(responseCode = "400", description = "参数错误"),
            @ApiResponse(responseCode = "404", description = "科室不存在")
    })
    public JsonVO<String> activeOrInactiveDepartment(@RequestBody List<Long> ids) {
        return JsonVO.success(null);
    }
}
