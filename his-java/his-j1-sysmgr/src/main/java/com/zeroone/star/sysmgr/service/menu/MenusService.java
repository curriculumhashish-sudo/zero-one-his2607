package com.zeroone.star.sysmgr.service.menu;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;

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
    MenuVO getMenuDetail(Long menuId);
}
