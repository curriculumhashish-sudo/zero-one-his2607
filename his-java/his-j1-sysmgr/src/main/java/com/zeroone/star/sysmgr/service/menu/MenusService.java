package com.zeroone.star.sysmgr.service.menu;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.menu.MenuCreateDTO;
import com.zeroone.star.project.dto.menu.MenuEditStatusDTO;
import com.zeroone.star.project.dto.menu.MenuUpdateDTO;
import com.zeroone.star.project.dto.menu.MenuDeleteDTO;
import com.zeroone.star.project.query.menu.MenuListPageQuery;
import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import com.zeroone.star.project.vo.JsonVO;

import java.util.List;

/**
 * @author xin
 * @description 针对表【whale_menus(菜单)】的数据库操作Service
 * @createDate 2026-07-23 22:43:12
 */
public interface MenusService extends IService<MenuDO> {

    /**
     * 获取菜单详情
     * @author Drums小白
     * @param menuId 菜单id
     * @return 菜单VO
     */
    MenuVO getMenuDetail(String menuId);

    /**
     * 创建菜单
     * @author 是丫丫
     * @param menuDto 菜单Dto
     * @return 是否创建成功
     */
    Boolean create(MenuCreateDTO menuDto);

    /**
     * 修改菜单状态  启用/停用菜单
     * @param menuDto 菜单启用入参
     * @return 是否修改成功
     * @author 是丫丫
     */
    Boolean editStatus(MenuEditStatusDTO menuDto);

    /**
     * 修改菜单基础信息
     * @author Drums小白
     * @param editDTO 修改菜单Dto
     * @return 是否修改成功
     */
    Boolean editMenu(MenuUpdateDTO editDTO);

    /**
     * 删除菜单信息
     * @param menuDto 菜单删除入参
     * @return 是否删除成功
     * @author 是丫丫
     * @负责人 Drums小白
     */
    JsonVO<Boolean> delete(MenuDeleteDTO menuDto);

    /**
     * 获取菜单树
     * @return 菜单树VO
     * @author osillow
     * @date 2026/7/25 18:21
     */
    List<MenuTreeVO> getMenuTree(List<String> roles);

    /**
     * 分页获取当前角色可访问的菜单列表
     * @param roles 当前用户角色列表
     * @param query 查询条件
     * @return 菜单分页列表
     */
    PageDTO<MenuVO> getMenuList(List<String> roles, MenuListPageQuery query);
}
