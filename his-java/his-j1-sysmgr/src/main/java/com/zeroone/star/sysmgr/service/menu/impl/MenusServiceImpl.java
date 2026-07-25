package com.zeroone.star.sysmgr.service.menu.impl;

import cn.hutool.core.lang.Assert;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.menu.MenuCreateDTO;
import com.zeroone.star.project.dto.menu.MenuEditStatusDTO;
import com.zeroone.star.project.dto.menu.MenuUpdateDTO;
import com.zeroone.star.sysmgr.convertor.MenuConvertor;
import com.zeroone.star.project.dto.menu.MenuDeleteDTO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.sysmgr.exception.MenuStatus;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.mapper.menu.MenusMapper;
import com.zeroone.star.sysmgr.service.menu.MenusService;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import com.zeroone.star.project.vo.JsonVO;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;

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
        // 校验菜单是否已经存在
        LambdaQueryWrapper<MenuDO> wrapper = new LambdaQueryWrapper<>();
        // 最终sql： WHERE path = ? AND （code = ? OR name = ? OR display_name = ?）
        wrapper.eq(MenuDO::getPath, menuDto.getPath())
                .and(
                        w -> w.eq(MenuDO::getCode, menuDto.getCode()).or()
                                .eq(MenuDO::getName, menuDto.getName()).or()
                                .eq(MenuDO::getDisplayName, menuDto.getDisplayName())
                );
        if (this.baseMapper.selectCount(wrapper) > 0) {
            throw new SystemException(MenuStatus.MENU_ALREADY_IS_EXIST);
        }
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
     * 修改菜单基础信息
     * @author Drums小白
     * @param editDTO 修改菜单Dto
     * @return 是否修改成功
     */
    @Override
    public Boolean editMenu(MenuUpdateDTO editDTO) {
        // 1. 查询菜单，校验菜单是否存在（过滤已删除数据）
        LambdaQueryWrapper<MenuDO> existWrapper = new LambdaQueryWrapper<>();
        existWrapper.eq(MenuDO::getId, editDTO.getId())
                .eq(MenuDO::getIsDeleted, 0);
        MenuDO menuDO = baseMapper.selectOne(existWrapper);
        if (menuDO == null) {
            throw new SystemException(MenuStatus.MENU_NOT_FIND);
        }

        // 2. 校验路径、编码、名称冲突，排除自身菜单
        LambdaQueryWrapper<MenuDO> repeatWrapper = new LambdaQueryWrapper<>();
        repeatWrapper.eq(MenuDO::getPath, editDTO.getPath())
                .and(w -> w.eq(MenuDO::getCode, editDTO.getCode())
                        .or().eq(MenuDO::getName, editDTO.getName())
                        .or().eq(MenuDO::getDisplayName, editDTO.getDisplayName()))
                .ne(MenuDO::getId, editDTO.getId());
        if (baseMapper.selectCount(repeatWrapper) > 0) {
            throw new SystemException(MenuStatus.MENU_ALREADY_IS_EXIST);
        }

        // 3. DTO转换DO
        MenuDO updateDO = MenuConvertor.INSTANCE.updateDO(editDTO);

        // 4. 处理清空父菜单标识
        if (Boolean.TRUE.equals(editDTO.getClearParent())) {
            updateDO.setParentId("");
        }

        // 5. 执行更新
        int result = baseMapper.updateById(updateDO);
        Assert.isTrue(result > 0, () -> new SystemException(MenuStatus.MENU_EDIT_FAIL));
        return result > 0;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public JsonVO<Boolean> delete(MenuDeleteDTO menuDto) {
        String menuId = menuDto.getId();
        // 1. 查询菜单
        MenuDO menu = baseMapper.selectById(menuId);
        if (menu == null || Integer.valueOf(1).equals(menu.getIsDeleted())) {
            throw new SystemException(MenuStatus.MENU_NOT_FIND);
        }

        // 2. 校验是否存在有效子菜单
        long childNum = baseMapper.selectCount(
                new LambdaQueryWrapper<MenuDO>()
                        .eq(MenuDO::getParentId, menuId)
                        .eq(MenuDO::getIsDeleted, 0)
        );
        if (childNum > 0) {
            throw new SystemException(MenuStatus.MENU_HAS_CHILD);
        }

        // 修改删除时间
        LambdaUpdateWrapper<MenuDO> wrapper = new LambdaUpdateWrapper<>();
        wrapper.eq(MenuDO::getId, menuId);
        // TODO: 暂时写死
        wrapper.set(MenuDO::getDeletedBy, "1");
        wrapper.set(MenuDO::getDeletedAt, LocalDateTime.now());
        int result = baseMapper.update(wrapper);
        Assert.isTrue(result > 0, () -> new SystemException(MenuStatus.MENU_EDIT_FAIL));

        // 3. 执行软删除
        int rows = baseMapper.deleteById(menuId);
        return JsonVO.success(rows > 0);
    }
}
