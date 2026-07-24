package com.zeroone.star.project.j1.permission;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.permission.DeleteResult;
import com.zeroone.star.project.dto.j1.permission.PermissionCreateDTO;
import com.zeroone.star.project.dto.j1.permission.PermissionUpdateDTO;
import com.zeroone.star.project.query.j1.permission.PermissionQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.permission.PermissionTreeVO;
import com.zeroone.star.project.vo.j1.permission.PermissionVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.parameters.RequestBody;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;

import java.util.List;

/**
 * 权限管理接口定义。
 *
 * @author j1-权限管理组
 * @version 1.0.0
 */
@Tag(name = "J1-权限管理", description = "权限树查询、分页查询、新增、修改和级联删除接口")
public interface PermissionApis {

    @Operation(summary = "获取权限名称树")
    JsonVO<List<PermissionTreeVO>> getPermissionTree();

    @Operation(summary = "条件分页查询权限列表")
    JsonVO<PageDTO<PermissionVO>> queryPermissions(@Valid PermissionQuery query);

    @Operation(summary = "新增权限")
    JsonVO<PermissionVO> createPermission(
            @RequestBody(description = "新增权限数据", required = true) @Valid PermissionCreateDTO dto);

    @Operation(summary = "编辑权限")
    JsonVO<PermissionVO> modifyPermission(
            @Parameter(description = "权限编号", required = true, example = "329631932614709248")
            @NotBlank(message = "权限编号不能为空") String id,
            @RequestBody(description = "修改权限数据", required = true) @Valid PermissionUpdateDTO dto);

    @Operation(summary = "删除权限（级联）", description = "软删除指定权限及其所有子权限，同时删除对应的授权记录")
    JsonVO<DeleteResult> removePermission(
            @Parameter(description = "权限编号", required = true, example = "329631932614709248")
            @NotBlank(message = "权限编号不能为空") String id);
}
