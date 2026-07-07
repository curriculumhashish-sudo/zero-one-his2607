package com.zeroone.star.project.menu;

import com.zeroone.star.project.dto.menu.*;
import com.zeroone.star.project.query.menu.MenuDetailQuery;
import com.zeroone.star.project.query.menu.MenuListPageQuery;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.project.vo.menu.MenuVO;

/**
 * 菜单管理的api接口
 * @author 是丫丫
 * @date  2026-07-19
 */
public interface MenuApis {

    /**
     * 加载菜单树结构
     * @return 菜单树结构VO
     * @author 是丫丫
     */
    default JsonVO<MenuTreeVO> getMenuTree() {
        return JsonVO.success(null);
    }

    /**
     * 获取菜单列表（条件 + 分页）
     * @param query 查询条件
     * @return 菜单分页列表
     * @author 是丫丫
     */
    default JsonVO<PageDTO<MenuVO>> getMenuList(MenuListPageQuery query) {
        return JsonVO.success(null);
    }

    /**
     * 加载菜单详情信息
     * @param query 查询条件
     * @return 菜单详情信息VO
     * @author 是丫丫
     */
    default JsonVO<MenuVO> getMenu(MenuDetailQuery query) {
        return JsonVO.success(null);
    }

    /**
     * 加载用户可见菜单树结构
     * @return 菜单树结构VO
     * @author 是丫丫
     */
    default JsonVO<MenuTreeVO> getTreeByLoginUser() {
        return JsonVO.success(null);
    }

    /**
     * 创建菜单信息
     * @param menuDto 菜单创建入参
     * @return 是否创建成功
     * @author 是丫丫
     */
    public JsonVO<Boolean> create(MenuCreateDTO menuDto);

    /**
     * 更新菜单信息
     * @param menuDto 菜单修改入参
     * @return 是否修改成功
     * @author 是丫丫
     */
    public JsonVO<Boolean> update(MenuUpdateRequestDTO menuDto);

    /**
     * 删除菜单信息
     * @param menuDto 菜单删除入参
     * @return 是否删除成功
     * @author 是丫丫
     */
    public JsonVO<Boolean> delete(MenuDeleteDTO menuDto);

    /**
     * 启用菜单
     * @param menuDto 菜单启用入参
     * @return 是否启用成功
     * @author 是丫丫
     */
    public JsonVO<Boolean> enable(MenuEnableDTO menuDto);

    /**
     * 禁用菜单
     * @param menuDto 菜单禁用入参
     * @return 是否禁用成功
     * @author 是丫丫
     */
    public JsonVO<Boolean> disable(MenuDisableDTO menuDto);

    /**
     * 提交指定角色的菜单权限
     * @param rolePermissionAssignDTO 权限提交入参
     * @return 是否添加成功
     * @author 是丫丫
     */
    public JsonVO<Boolean> assignPermissions(RolePermissionAssignDTO rolePermissionAssignDTO);
}
