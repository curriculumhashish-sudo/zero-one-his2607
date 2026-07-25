package com.zeroone.star.sysmgr.service.menu.impl;

import cn.hutool.core.lang.Assert;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.menu.MenuCreateDTO;
import com.zeroone.star.project.dto.menu.MenuEditStatusDTO;
import com.zeroone.star.project.dto.menu.MenuUpdateDTO;
import com.zeroone.star.project.query.menu.MenuListPageQuery;
import com.zeroone.star.project.utils.tree.TreeUtils;
import com.zeroone.star.project.vo.login.MenuTreeVO;
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
import org.springframework.util.StringUtils;

import java.util.Comparator;
import java.util.List;
import java.util.Locale;

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

        // 3. 执行软删除
        MenuDO updateDO = new MenuDO();
        updateDO.setId(menuId);
        updateDO.setIsDeleted(1);
        int rows = baseMapper.updateById(updateDO);
        return JsonVO.success(rows > 0);
    }

    /**
     * @description
     * @param roles 角色列表
     * @return 菜单树
     * @author osillow
     * @date 2026/7/25 21:28
     */
    @Override
    public List<MenuTreeVO> getMenuTree(List<String> roles) {
        if (roles == null || roles.isEmpty()) {
            return List.of();
        }
        List<MenuDO> menuList = menusMapper.selectMenusByRoles(roles);
        return buildMenuTree(menuList);
    }

    @Override
    public PageDTO<MenuVO> getMenuList(List<String> roles, MenuListPageQuery query) {
        PageDTO<MenuVO> pageDTO = new PageDTO<>();
        long pageNo = resolvePageNo(query);
        long pageSize = resolvePageSize(query);
        pageDTO.setPageIndex(pageNo);
        pageDTO.setPageSize(pageSize);

        if (roles == null || roles.isEmpty()) {
            pageDTO.setTotal(0L);
            pageDTO.setPages(0L);
            pageDTO.setRows(List.of());
            return pageDTO;
        }

        List<MenuVO> filtered = menusMapper.selectMenusByRoles(roles).stream()
                .filter(menu -> matchKeyword(menu, query == null ? null : query.getKeyword()))
                .sorted(Comparator
                        .comparing(MenuDO::getSortOrder, Comparator.nullsLast(Integer::compareTo))
                        .thenComparing(MenuDO::getId, Comparator.nullsLast(String::compareTo)))
                .map(this::toMenuVO)
                .toList();

        long total = filtered.size();
        long pages = total == 0 ? 0 : (total + pageSize - 1) / pageSize;
        long fromIndex = Math.min((pageNo - 1) * pageSize, total);
        long toIndex = Math.min(fromIndex + pageSize, total);

        pageDTO.setTotal(total);
        pageDTO.setPages(pages);
        pageDTO.setRows(filtered.subList((int) fromIndex, (int) toIndex));
        return pageDTO;
    }

    private List<MenuTreeVO> buildMenuTree(List<MenuDO> menuDOList){
        if (menuDOList == null || menuDOList.isEmpty()) {
            return List.of();
        }

        return TreeUtils.listToTree(menuDOList, menu -> {
            MenuTreeVO node = new MenuTreeVO();
            node.setTnId(menu.getId());
            node.setTnPid(menu.getParentId());
            node.setId(toInteger(menu.getId()));
            node.setText(menu.getDisplayName());
            node.setIcon(menu.getIcon());
            node.setHref(menu.getPath());
            node.setPid(toInteger(menu.getParentId()));
            return node;
        });
    }


    /**
     * @description
     * @param value String类
     * @return Integer包装类
     * @author osillow
     * @date 2026/7/25 21:30
     */
    private Integer toInteger(String value) {
        if (value == null || value.isBlank()) {
            return null;
        }
        try {
            return Integer.valueOf(value);
        } catch (NumberFormatException e) {
            return null;
        }
    }

    private long resolvePageNo(MenuListPageQuery query) {
        if (query == null) {
            return 1;
        }
        return query.getPageIndex() > 0 ? query.getPageIndex() : 1;
    }

    private long resolvePageSize(MenuListPageQuery query) {
        if (query == null || query.getPageSize() <= 0) {
            return 10;
        }
        return query.getPageSize();
    }

    private boolean matchKeyword(MenuDO menu, String keyword) {
        if (!StringUtils.hasText(keyword)) {
            return true;
        }
        String normalized = keyword.toLowerCase(Locale.ROOT);
        return containsIgnoreCase(menu.getCode(), normalized)
                || containsIgnoreCase(menu.getName(), normalized)
                || containsIgnoreCase(menu.getDisplayName(), normalized)
                || containsIgnoreCase(menu.getPath(), normalized)
                || containsIgnoreCase(menu.getComponent(), normalized)
                || containsIgnoreCase(menu.getPermissionName(), normalized);
    }

    private boolean containsIgnoreCase(String value, String normalizedKeyword) {
        return value != null && value.toLowerCase(Locale.ROOT).contains(normalizedKeyword);
    }
    /**
     * @description
     * @param menu menuDO
     * @return vo
     * @author osillow
     * @date 2026/7/25 21:30
     */
    private MenuVO toMenuVO(MenuDO menu) {
        MenuVO vo = new MenuVO();
        vo.setMenuId(menu.getId());
        vo.setMenuName(menu.getName());
        vo.setShowName(menu.getDisplayName());
        vo.setIcon(menu.getIcon());
        vo.setPath(menu.getPath());
        vo.setComponent(menu.getComponent());
        vo.setParentId(menu.getParentId());
        vo.setSortNum(menu.getSortOrder() == null ? null : menu.getSortOrder().longValue());
        vo.setMenuType(menu.getMenuType() == null ? null : menu.getMenuType().name());
        vo.setStatus(menu.getIsEnabled() == null ? null : menu.getIsEnabled().toString());
        return vo;
    }
}
