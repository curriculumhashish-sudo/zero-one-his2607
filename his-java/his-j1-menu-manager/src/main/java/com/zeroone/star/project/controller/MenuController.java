package com.zeroone.star.project.controller;

import com.zeroone.star.project.dto.menu.*;
import com.zeroone.star.project.menu.MenuApis;
import com.zeroone.star.project.query.menu.MenuDetailQuery;
import com.zeroone.star.project.query.menu.MenuTreeQuery;
import com.zeroone.star.project.service.IMenuService;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.project.vo.menu.MenuVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 菜单管理的api接口
 * @author 是丫丫
 * @date  2026-07-19
 */
@RestController
@RequestMapping("menu")
@Tag(name = "菜单管理")
public class MenuController implements MenuApis {

    @Resource
    private IMenuService menuService;

    /**
     * 加载菜单树结构
     * @param query 查询条件
     * @return 菜单树结构VO
     * @author 是丫丫
     */
    @Override
    @GetMapping("getTree")
    @Operation(summary = "加载菜单树结构")
    public JsonVO<MenuTreeVO> getMenuTree(@Valid MenuTreeQuery query) {
        return null;
    }

    /**
     * 加载菜单详情信息
     * @param query 查询条件
     * @return 菜单详情信息VO
     * @author 是丫丫
     */
    @Override
    @GetMapping("getById")
    @Operation(summary = "加载菜单详情信息")
    public JsonVO<MenuVO> getMenu(@Valid MenuDetailQuery query) {
        return null;
    }

    /**
     * <p>
     *     根据当前登录用户来查询可见菜单树结构 <br>
     *     注：这里要注意做登录校验，如果获取不到用户登录信息，直接返回空列表
     * </p>
     * @see com.zeroone.star.project.components.user.UserHolder#getCurrentUser()
     * @param query 查询条件
     * @return 菜单树结构VO
     */
    @Override
    @GetMapping("getTreeByLoginUser")
    @Operation(summary = "根据当前登录用户来查询可见菜单树结构")
    public JsonVO<MenuTreeVO> getTreeByLoginUser(@Valid MenuTreeQuery query) {
        List<MenuTreeVO> tree = menuService.listMenuTreeByLoginUser();
        if (tree == null || tree.isEmpty()) {
            return JsonVO.success(null);
        }
        if (tree.size() == 1) {
            return JsonVO.success(tree.get(0));
        }
        // 多棵树：创建虚拟根节点合并
        MenuTreeVO virtualRoot = new MenuTreeVO();
        virtualRoot.setText("根菜单");
        for (MenuTreeVO node : tree) {
            virtualRoot.addChild(node);
        }
        return JsonVO.success(virtualRoot);
    }

    /**
     * 创建菜单信息
     * @param menuDto 菜单创建入参
     * @return 是否创建成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("create")
    @Operation(summary = "创建菜单信息")
    public JsonVO<Boolean> create(@Valid MenuCreateDTO menuDto) {
        return null;
    }

    /**
     * 更新菜单信息
     * @param menuDto 菜单修改入参
     * @return 是否修改成功
     * @author 是丫丫
     */
    @Override
    @PutMapping("update")
    @Operation(summary = "更新菜单信息")
    public JsonVO<Boolean> update(@Valid MenuUpdateDTO menuDto) {
        return null;
    }

    /**
     * 删除菜单信息
     * @param menuDto 菜单删除入参
     * @return 是否删除成功
     * @author 是丫丫
     */
    @Override
    @DeleteMapping("delete")
    @Operation(summary = "删除菜单信息")
    public JsonVO<Boolean> delete(@Valid MenuDeleteDTO menuDto) {
        return null;
    }

    /**
     * 启用菜单
     * @param menuDto 菜单启用入参
     * @return 是否启用成功
     * @author 是丫丫
     */
    @Override
    @PutMapping("enable")
    @Operation(summary = "启用菜单")
    public JsonVO<Boolean> enable(@Valid MenuEnableDTO menuDto) {
        return null;
    }

    /**
     * 禁用菜单
     * @param menuDto 菜单禁用入参
     * @return 是否禁用成功
     * @author 是丫丫
     */
    @Override
    @PutMapping("disable")
    @Operation(summary = "禁用菜单")
    public JsonVO<Boolean> disable(@Valid MenuDisableDTO menuDto) {
        return null;
    }

    /**
     * 提交指定角色的菜单权限
     * @param rolePermissionAssignDTO 权限提交入参
     * @return 是否添加成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("assignPermissions")
    @Operation(summary = "提交指定角色的菜单权限")
    public JsonVO<Boolean> assignPermissions(RolePermissionAssignDTO rolePermissionAssignDTO) {
        boolean success = menuService.assignPermissions(rolePermissionAssignDTO);
        return JsonVO.success(success);
    }
}
