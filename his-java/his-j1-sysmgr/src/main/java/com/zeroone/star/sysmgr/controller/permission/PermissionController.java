package com.zeroone.star.sysmgr.controller.permission;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.permission.DeleteResult;
import com.zeroone.star.project.dto.j1.permission.PermissionCreateDTO;
import com.zeroone.star.project.dto.j1.permission.PermissionUpdateDTO;
import com.zeroone.star.project.j1.permission.PermissionApis;
import com.zeroone.star.project.query.j1.permission.PermissionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.permission.PermissionTreeVO;
import com.zeroone.star.project.vo.j1.permission.PermissionVO;
import com.zeroone.star.sysmgr.service.permission.IPermissionService;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@Validated
@RestController
@RequestMapping("/permission")
public class PermissionController implements PermissionApis {

    @Resource
    private IPermissionService permissionService;

    @Override
    @GetMapping("/tree")
    public JsonVO<List<PermissionTreeVO>> getPermissionTree() {
        return JsonVO.success(permissionService.listPermissionTree());
    }

    @Override
    @GetMapping("/list")
    public JsonVO<PageDTO<PermissionVO>> queryPermissions(@Valid PermissionQuery query) {
        return JsonVO.success(permissionService.listPermissions(query));
    }

    @Override
    @PostMapping
    public JsonVO<PermissionVO> createPermission(@Valid @RequestBody PermissionCreateDTO dto) {
        return JsonVO.success(permissionService.createPermission(dto));
    }

    @Override
    @PutMapping("/{id}")
    public JsonVO<PermissionVO> modifyPermission(@PathVariable @NotBlank(message = "权限编号不能为空") String id,
                                                  @Valid @RequestBody PermissionUpdateDTO dto) {
        return JsonVO.success(permissionService.updatePermission(id, dto));
    }

    @Override
    @DeleteMapping("/{id}")
    public JsonVO<DeleteResult> removePermission(@PathVariable @NotBlank(message = "权限编号不能为空") String id) {
        return JsonVO.success(permissionService.deletePermission(id));
    }
}