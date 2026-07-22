package com.zeroone.star.sysmgr.controller.role;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.role.RoleAddDTO;
import com.zeroone.star.project.dto.j1.role.RoleModifyDTO;
import com.zeroone.star.project.j1.role.RoleApis;
import com.zeroone.star.project.query.j1.role.RoleQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.role.RoleNameVO;
import com.zeroone.star.project.vo.j1.role.RoleVO;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：角色管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j1-保温杯热水仙人
 * @version 1.0.0
 */
@Validated
@RestController
public class RoleController implements RoleApis {

    @Override
    public JsonVO<List<RoleNameVO>> listRoleNames() {
        return null;
    }

    @Override
    public JsonVO<PageDTO<RoleVO>> queryRoles(RoleQuery query) {
        return null;
    }

    @Override
    public JsonVO<RoleVO> getRole(String id) {
        return null;
    }

    @Override
    public JsonVO<String> addRole(RoleAddDTO dto) {
        return null;
    }

    @Override
    public JsonVO<String> modifyRole(RoleModifyDTO dto) {
        return null;
    }

    @Override
    public JsonVO<String> removeRole(String id) {
        return null;
    }
}
