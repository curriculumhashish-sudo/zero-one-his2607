package com.zeroone.star.sysmgr.service.menu;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.project.dto.menu.MenuCreateDTO;
import com.zeroone.star.project.dto.menu.MenuEditStatusDTO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import com.zeroone.star.project.vo.menu.MenuVO;

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
}
