package com.zeroone.star.sysmgr.service.menu.impl;

import cn.hutool.core.lang.Assert;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.menu.MenuCreateDTO;
import com.zeroone.star.project.dto.menu.MenuEditStatusDTO;
import com.zeroone.star.sysmgr.convertor.MenuConvertor;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.sysmgr.exception.MenuStatus;
import com.zeroone.star.sysmgr.exception.SystemException;
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
    public MenuVO getMenuDetail(String menuId) {
        LambdaQueryWrapper<MenuDO> wrapper = new LambdaQueryWrapper<>();
        // 主键id 继承BaseDO
        wrapper.eq(MenuDO::getId, menuId);
        // 软删除 0正常
        wrapper.eq(MenuDO::getIsDeleted, 0);
        MenuDO menuDO = menusMapper.selectOne(wrapper);

        if (menuDO == null) {
            throw new SystemException(MenuStatus.MENU_NOT_FIND);
        }
        return MenuConvertor.INSTANCE.toVO(menuDO);
    }

    @Override
    public Boolean create(MenuCreateDTO menuDto) {
        MenuDO menuDO = MenuConvertor.INSTANCE.createDO(menuDto);
        int result = this.baseMapper.insert(menuDO);
        Assert.isTrue(result > 0, () -> new SystemException(MenuStatus.MENU_CREATE_FAIL));
        return result > 0;
    }

    @Override
    public Boolean editStatus(MenuEditStatusDTO menuDto) {
        LambdaUpdateWrapper<MenuDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(MenuDO::getId, menuDto.getId());
        wrapper.eq(MenuDO::getIsDeleted, 0);
        wrapper.set(MenuDO::getIsEnabled, menuDto.getStatus());
        int result = this.baseMapper.update(wrapper);
        Assert.isTrue(result > 0, () -> new SystemException(MenuStatus.MENU_STATUS_EDIT_FAIL));
        return result > 0;
    }

    /**
     * DO 对象转为 VO 对象
     * @deprecated
     * @param menuDO 查询条件
     * @return 菜单详情信息VO
     */
    @Deprecated
    private MenuVO convertToVO(MenuDO menuDO) {
        MenuVO vo = new MenuVO();
        vo.setMenuId(menuDO.getId());
        vo.setMenuName(menuDO.getName());
        vo.setShowName(menuDO.getDisplayName());
        vo.setIcon(menuDO.getIcon());
        vo.setPath(menuDO.getPath());
        vo.setComponent(menuDO.getComponent());

        if (menuDO.getParentId() != null && !menuDO.getParentId().isEmpty()) {
            vo.setParentId(menuDO.getParentId());
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
