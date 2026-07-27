package com.zeroone.star.sysmgr.controller.role;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.role.RoleAddDTO;
import com.zeroone.star.project.dto.j1.role.RoleModifyDTO;
import com.zeroone.star.project.j1.role.RoleApis;
import com.zeroone.star.project.query.j1.role.RoleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.role.RoleNameVO;
import com.zeroone.star.project.vo.j1.role.RoleVO;
import com.zeroone.star.sysmgr.service.role.IRoleService;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * 角色管理控制器
 * <p>
 * 实现角色管理接口并统一封装接口响应
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
@Validated
@RestController
public class RoleController implements RoleApis {

    @Resource
    private IRoleService roleService;

    @Override
    public JsonVO<List<RoleNameVO>> listRoleNames() {
        return JsonVO.success(roleService.listRoleNames());
    }

    @Override
    public JsonVO<PageDTO<RoleVO>> queryRoles(RoleQuery query) {
        return JsonVO.success(roleService.queryRoles(query));
    }

    @Override
    public JsonVO<RoleVO> getRole(String id) {
        return JsonVO.success(roleService.getRole(id));
    }

    @Override
    public JsonVO<String> addRole(RoleAddDTO dto) {
        return JsonVO.success(roleService.addRole(dto));
    }

    @Override
    public JsonVO<String> modifyRole(RoleModifyDTO dto) {
        roleService.modifyRole(dto);
        return JsonVO.success("修改成功");
    }

    @Override
    public JsonVO<String> removeRole(String id) {
        roleService.removeRole(id);
        return JsonVO.success("删除成功");
    }
}
