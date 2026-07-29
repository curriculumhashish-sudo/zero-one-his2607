package com.zeroone.star.sysmgr.service.role;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.role.RoleAddDTO;
import com.zeroone.star.project.dto.j1.role.RoleModifyDTO;
import com.zeroone.star.project.query.j1.role.RoleQuery;
import com.zeroone.star.project.vo.j1.role.RoleNameVO;
import com.zeroone.star.project.vo.j1.role.RoleVO;
import com.zeroone.star.sysmgr.entity.role.RoleDO;

import java.util.List;

/**
 * 角色管理业务服务
 * <p>
 * 统一封装角色的租户可见性、基本信息维护及安全删除规则
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
public interface IRoleService extends IService<RoleDO> {

    /**
     * 获取当前租户可见的角色名称列表。
     *
     * @return 角色名称列表
     */
    List<RoleNameVO> listRoleNames();

    /**
     * 分页查询当前租户可见的角色。
     *
     * @param query 查询条件
     * @return 角色分页数据
     */
    PageDTO<RoleVO> queryRoles(RoleQuery query);

    /**
     * 获取角色详情。
     *
     * @param id 角色编号
     * @return 角色详情
     */
    RoleVO getRole(String id);

    /**
     * 新增当前租户的自定义角色。
     *
     * @param dto 新增角色数据
     * @return 新增后的角色详情
     */
    RoleVO addRole(RoleAddDTO dto);

    /**
     * 修改角色基本信息。
     * <p>
     * 内置角色和自定义角色均可修改基本信息；admin 的权限信息由角色授权流程限制。
     * </p>
     *
     * @param dto 修改角色数据
     * @return 修改后的角色详情
     */
    RoleVO modifyRole(RoleModifyDTO dto);

    /**
     * 删除当前租户的自定义角色。
     *
     * @param id 角色编号
     */
    void removeRole(String id);
}
