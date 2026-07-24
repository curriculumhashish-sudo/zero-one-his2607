package com.zeroone.star.sysmgr.service.menu.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.entity.menu.MenuDO;
import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.sysmgr.mapper.menu.MenusMapper;
import com.zeroone.star.sysmgr.service.menu.MenusService;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

/**
 * @author xin
 * @description 针对表【whale_menus(菜单)】的数据库操作Service实现
 * @createDate 2026-07-23 22:43:12
 * @author Drums小白
 * @date 2026-07-24
 */
@Service
@RequiredArgsConstructor
public class MenusServiceImpl extends ServiceImpl<MenusMapper, MenuDO> implements MenusService {

    private final MenusMapper menusMapper;

    @Override
    public MenuVO getMenuDetail(Long menuId) {
        LambdaQueryWrapper<MenuDO> wrapper = new LambdaQueryWrapper<>();
        // 根据菜单ID查询，过滤逻辑删除的数据
        wrapper.eq(MenuDO::getMenuId, menuId);
        wrapper.eq(MenuDO::getDeleted, 0);
        MenuDO menuDO = menusMapper.selectOne(wrapper);

        // 判断菜单是否存在
        if (menuDO == null) {
            throw new RuntimeException("菜单信息不存在");
        }
        // DO转换为VO返回
        return convertToVO(menuDO);
    }

    /**
     * DO 对象转为 VO 对象
     */
    private MenuVO convertToVO(MenuDO menuDO) {
        MenuVO vo = new MenuVO();
        vo.setMenuId(menuDO.getMenuId());
        vo.setMenuName(menuDO.getMenuName());
        vo.setShowName(menuDO.getShowName());
        vo.setIcon(menuDO.getIcon());
        vo.setPath(menuDO.getPath());
        vo.setComponent(menuDO.getComponent());
        vo.setParentId(menuDO.getParentId());
        vo.setSort(menuDO.getSort());
        vo.setSortNum(menuDO.getSortNum());
        vo.setMenuType(menuDO.getMenuType());
        // 状态转换：DO:1启用 0停用  → VO:"0"启用 "1"停用
        vo.setStatus(menuDO.getEnabled() == 1 ? "0" : "1");
        return vo;
    }
}
