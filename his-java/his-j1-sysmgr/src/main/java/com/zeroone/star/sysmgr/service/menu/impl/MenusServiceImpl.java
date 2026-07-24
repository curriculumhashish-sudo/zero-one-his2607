package com.zeroone.star.sysmgr.service.menu.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
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
        // 主键id 继承BaseDO
        wrapper.eq(MenuDO::getId, menuId);
        // 软删除 0正常
        wrapper.eq(MenuDO::getIsDeleted, 0);
        MenuDO menuDO = menusMapper.selectOne(wrapper);

        if (menuDO == null) {
            throw new RuntimeException("菜单信息不存在");
        }
        return convertToVO(menuDO);
    }

    /**
     * DO 对象转为 VO 对象
     */
    /**
     * DO 对象转为 VO 对象
     */
    /**
     * DO 对象转为 VO 对象
     */
    /**
     * DO 对象转为 VO 对象
     */
    /**
     * DO 对象转为 VO 对象
     */
    /**
     * DO 对象转为 VO 对象
     * @param menuDO 查询条件
     * @return 菜单详情信息VO
     */
    private MenuVO convertToVO(MenuDO menuDO) {
        MenuVO vo = new MenuVO();
        vo.setMenuId(Long.valueOf(menuDO.getId()));
        vo.setMenuName(menuDO.getName());
        vo.setShowName(menuDO.getDisplayName());
        vo.setIcon(menuDO.getIcon());
        vo.setPath(menuDO.getPath());
        vo.setComponent(menuDO.getComponent());

        // parentId DO为String，VO为Long
        if (menuDO.getParentId() != null && !menuDO.getParentId().isEmpty()) {
            vo.setParentId(Long.valueOf(menuDO.getParentId()));
        } else {
            vo.setParentId(null);
        }

        vo.setSortNum(Long.valueOf(menuDO.getSortOrder()));
        vo.setMenuType(menuDO.getMenuType());
        // 状态映射：DO 1启用 0禁用 → VO "0"启用 "1"禁用
        vo.setStatus(menuDO.getIsEnabled() == 1 ? "0" : "1");
        return vo;
    }
}
