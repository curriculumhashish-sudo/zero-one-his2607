package com.zeroone.star.sysmgr.service.menu.impl;

import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.sysmgr.entity.menu.MenuDO;
import com.zeroone.star.sysmgr.mapper.menu.MenuMapper;
import com.zeroone.star.sysmgr.service.menu.MenuService;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;

import java.util.*;

/**
 * 菜单服务实现。
 */
@Slf4j
@Service
@RequiredArgsConstructor
public class MenuServiceImpl implements MenuService {

    private final MenuMapper menuMapper;

    @Override
    public List<MenuTreeVO> getMenuTree(List<String> roles) {
        if (roles == null || roles.isEmpty()) {
            return Collections.emptyList();
        }
        List<MenuDO> menus = menuMapper.selectMenusByRoles(roles);
        if (menus.isEmpty()) {
            return Collections.emptyList();
        }
        return buildTree(menus, null);
    }

    /**
     * 递归构建菜单树。
     *
     * @param allMenus 全部菜单列表
     * @param parentId 当前父节点 ID（null 表示根节点）
     * @return 子树
     */
    private List<MenuTreeVO> buildTree(List<MenuDO> allMenus, String parentId) {
        List<MenuTreeVO> tree = new ArrayList<>();

        for (MenuDO menu : allMenus) {
            // 根节点：parentId 为空、"0"、或等于自身 id
            boolean isRootChild = parentId == null
                    && (menu.getParentId() == null || menu.getParentId().isEmpty() || "0".equals(menu.getParentId()));
            boolean isChild = parentId != null && parentId.equals(menu.getParentId());

            if (!isRootChild && !isChild) {
                continue;
            }

            MenuTreeVO vo = toTreeVO(menu);
            List<MenuTreeVO> children = buildTree(allMenus, menu.getId());
            for (MenuTreeVO child : children) {
                vo.addChild(child);
            }
            tree.add(vo);
        }

        // 按 sortOrder 排序
        tree.sort(Comparator.comparing(
                vo -> {
                    MenuDO src = findMenuById(allMenus, vo.getId());
                    return src != null && src.getSortOrder() != null ? src.getSortOrder() : Integer.MAX_VALUE;
                }
        ));

        return tree;
    }

    private MenuTreeVO toTreeVO(MenuDO menu) {
        MenuTreeVO vo = new MenuTreeVO();
        vo.setId(menu.getId());
        vo.setText(menu.getDisplayName() != null ? menu.getDisplayName() : menu.getName());
        vo.setIcon(menu.getIcon());
        vo.setHref(menu.getPath());
        vo.setPid(menu.getParentId());
        // TreeNode 基类字段
        vo.setTnId(menu.getId());
        vo.setTnPid(menu.getParentId());
        return vo;
    }

    private MenuDO findMenuById(List<MenuDO> menus, String id) {
        return menus.stream().filter(m -> id.equals(m.getId())).findFirst().orElse(null);
    }
}
