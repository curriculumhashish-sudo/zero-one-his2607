package com.zeroone.star.sysmgr.controller.menu;

import com.zeroone.star.project.dto.menu.*;
import com.zeroone.star.project.menu.MenuApis;
import com.zeroone.star.project.query.menu.MenuDetailQuery;
import com.zeroone.star.project.query.menu.MenuTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.project.vo.menu.MenuVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

/**
 * 菜单管理的api接口
 * @author 是丫丫
 * @date  2026-07-19
 */
@RestController
@RequestMapping("menu")
@Tag(name = "菜单管理")
public class MenuController implements MenuApis {

    /**
     * 加载菜单树结构
     * @param query 查询条件
     * @return 菜单树结构VO
     * @author 是丫丫
     */
    @Override
    @GetMapping("getTree")
    @Operation(summary = "加载菜单树结构")
    public JsonVO<MenuTreeVO> getMenuTree(@RequestParam @Valid MenuTreeQuery query) {
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
    public JsonVO<MenuVO> getMenu(@RequestParam @Valid MenuDetailQuery query) {
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
    public JsonVO<MenuTreeVO> getTreeByLoginUser(@RequestParam @Valid MenuTreeQuery query) {
        return null;
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
    public JsonVO<Boolean> create(@RequestBody @Valid MenuCreateDTO menuDto) {
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
    public JsonVO<Boolean> update(@RequestBody @Valid MenuUpdateDTO menuDto) {
        return null;
    }

    /**
     * 删除菜单信息
     * @param menuDto 菜单删除入参
     * @return 是否删除成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("delete")
    @Operation(summary = "删除菜单信息")
    public JsonVO<Boolean> delete(@RequestBody @Valid MenuDeleteDTO menuDto) {
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
    public JsonVO<Boolean> enable(@RequestBody @Valid MenuEnableDTO menuDto) {
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
    public JsonVO<Boolean> disable(@RequestBody @Valid MenuDisableDTO menuDto) {
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
    public JsonVO<Boolean> assignPermissions(@RequestBody RolePermissionAssignDTO rolePermissionAssignDTO) {
        return null;
    }
}
