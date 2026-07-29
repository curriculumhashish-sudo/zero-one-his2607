package com.zeroone.star.project.j1.role;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.role.RoleAddDTO;
import com.zeroone.star.project.dto.j1.role.RoleModifyDTO;
import com.zeroone.star.project.query.j1.role.RoleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.role.RoleNameVO;
import com.zeroone.star.project.vo.j1.role.RoleVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import java.util.List;

/**
 * 角色管理接口
 * <p>
 * 定义角色查询、新增、修改和删除的统一接口契约
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Tag(name = "j1-角色管理", description = "角色查询、新增、修改和删除接口")
@RequestMapping("/role")
public interface RoleApis {

    /**
     * 获取角色名称列表。
     *
     * @return 角色名称列表
     */
    @Operation(summary = "获取角色名称列表", description = "获取未删除角色的编号和显示名称，用于角色选择组件")
    @GetMapping("/list-role-names")
    JsonVO<List<RoleNameVO>> listRoleNames();

    /**
     * 分页查询角色。
     *
     * @param query 角色查询条件
     * @return 角色分页数据
     */
    @Operation(summary = "分页查询角色")
    @PostMapping("/query-roles")
    JsonVO<PageDTO<RoleVO>> queryRoles(
            @Valid @RequestBody RoleQuery query);

    /**
     * 获取角色详情。
     *
     * @param id 角色编号
     * @return 角色详情
     */
    @Operation(summary = "获取角色详情")
    @GetMapping("/get-role")
    JsonVO<RoleVO> getRole(
            @Parameter(description = "角色编号", required = true, example = "329631932614709248")
            @RequestParam("id") @NotBlank(message = "角色编号不能为空") String id);

    /**
     * 新增角色。
     *
     * @param dto 新增角色数据
     * @return 新增后的角色详情
     */
    @Operation(summary = "新增角色", description = "角色内部名称和编码由服务端生成，成功后返回完整角色信息")
    @PostMapping("/add-role")
    JsonVO<RoleVO> addRole(
            @Valid @RequestBody RoleAddDTO dto);

    /**
     * 修改角色。
     *
     * @param dto 修改角色数据
     * @return 修改后的角色详情
     */
    @Operation(summary = "修改角色", description = "修改角色基本信息，角色内部名称和编码不允许修改，成功后返回完整角色信息")
    @PutMapping("/modify-role")
    JsonVO<RoleVO> modifyRole(
            @Valid @RequestBody RoleModifyDTO dto);

    /**
     * 删除角色。
     *
     * @param id 角色编号
     * @return 删除结果
     */
    @Operation(summary = "删除角色", description = "执行逻辑删除；内置静态角色不可删除")
    @DeleteMapping("/remove-role")
    JsonVO<String> removeRole(
            @Parameter(description = "角色编号", required = true, example = "329631932614709248")
            @RequestParam("id") @NotBlank(message = "角色编号不能为空") String id);
}
