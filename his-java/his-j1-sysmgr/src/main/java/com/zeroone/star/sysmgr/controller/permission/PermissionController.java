package com.zeroone.star.sysmgr.controller.permission;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.permission.*;
import com.zeroone.star.project.query.permission.PermissionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.permission.*;
import com.zeroone.star.sysmgr.service.IPermissionService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/permission")
@Tag(name = "权限管理")
public class PermissionController {

    @Resource
    private IPermissionService permissionService;

    @Operation(summary = "获取权限名称树")
    @GetMapping("/tree")
    public JsonVO<List<PermissionTreeVO>> getTree() {
        return JsonVO.success(permissionService.listPermissionTree());
    }

    @Operation(summary = "条件分页查询权限列表")
    @GetMapping("/list")
    public JsonVO<PageDTO<PermissionVO>> list(@Valid PermissionQuery query) {
        return JsonVO.success(permissionService.listPermissions(query));
    }

    @Operation(summary = "新增权限")
    @PostMapping
    public JsonVO<PermissionVO> create(@Valid @RequestBody PermissionCreateDTO dto) {
        return JsonVO.success(permissionService.createPermission(dto));
    }

    @Operation(summary = "编辑权限")
    @PutMapping("/{id}")
    public JsonVO<PermissionVO> update(@PathVariable String id,
                                       @Valid @RequestBody PermissionUpdateDTO dto) {
        return JsonVO.success(permissionService.updatePermission(id, dto));
    }

    @Operation(summary = "删除权限（级联）")
    @DeleteMapping("/{id}")
    public JsonVO<DeleteResult> delete(@PathVariable String id) {
        return JsonVO.success(permissionService.deletePermission(id));
    }
}