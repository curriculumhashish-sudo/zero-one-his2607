package com.zeroone.star.project.j1.role;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.role.RoleAddDTO;
import com.zeroone.star.project.dto.j1.role.RoleModifyDTO;
import com.zeroone.star.project.query.j1.role.RoleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.role.RoleVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.parameters.RequestBody;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;

/**
 * 角色管理接口定义。

 * @author 保温杯热水仙人
 * @version 1.0.0
 */
@Tag(name = "J1-角色管理", description = "角色查询、新增、修改和删除接口")
public interface RoleApis {

    @Operation(summary = "分页查询角色")
    JsonVO<PageDTO<RoleVO>> queryRoles(
            @RequestBody(description = "角色查询条件", required = true) @Valid RoleQuery query);

    @Operation(summary = "获取角色详情")
    JsonVO<RoleVO> getRole(
            @Parameter(description = "角色编号", required = true, example = "329631932614709248")
            @NotBlank(message = "角色编号不能为空") String id);

    @Operation(summary = "新增角色", description = "角色内部名称和编码由服务端生成")
    JsonVO<String> addRole(
            @RequestBody(description = "新增角色数据", required = true) @Valid RoleAddDTO dto);

    @Operation(summary = "修改角色", description = "角色内部名称和编码不允许修改")
    JsonVO<String> modifyRole(
            @RequestBody(description = "修改角色数据", required = true) @Valid RoleModifyDTO dto);

    @Operation(summary = "删除角色", description = "执行逻辑删除；内置静态角色不可删除")
    JsonVO<String> removeRole(
            @Parameter(description = "角色编号", required = true, example = "329631932614709248")
            @NotBlank(message = "角色编号不能为空") String id);

    @Operation(summary = "统计角色关联用户数")
    JsonVO<Long> countRoleUsers(
            @Parameter(description = "角色编号", required = true, example = "329631932614709248")
            @NotBlank(message = "角色编号不能为空") String id);
}
