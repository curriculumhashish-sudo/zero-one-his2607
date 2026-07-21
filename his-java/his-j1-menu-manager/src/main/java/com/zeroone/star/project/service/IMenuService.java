package com.zeroone.star.project.service;

import com.zeroone.star.project.dto.menu.RolePermissionAssignDTO;
import com.zeroone.star.project.vo.login.MenuTreeVO;

import java.util.List;

/**
 * 菜单服务接口
 * @author CatSN
 */
public interface IMenuService {

    /**
     * 获取当前登录用户的可见菜单树
     * @return 菜单树列表
     */
    List<MenuTreeVO> listMenuTreeByLoginUser();

    /**
     * 为指定角色分配菜单权限
     * @param dto 角色权限分配入参
     * @return 是否分配成功
     */
    boolean assignPermissions(RolePermissionAssignDTO dto);
}
