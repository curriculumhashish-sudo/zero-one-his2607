package com.zeroone.star.sysmgr.service.menu;

import com.zeroone.star.project.vo.login.MenuTreeVO;

import java.util.List;

/**
 * 菜单服务接口。
 */
public interface MenuService {

    /**
     * 根据用户角色获取菜单树。
     *
     * @param roles 角色列表（id/code/name 均可匹配）
     * @return 菜单树
     */
    List<MenuTreeVO> getMenuTree(List<String> roles);
}
