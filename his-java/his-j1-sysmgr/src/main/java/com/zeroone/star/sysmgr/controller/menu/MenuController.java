package com.zeroone.star.sysmgr.controller.menu;

import com.zeroone.star.project.dto.menu.*;
import com.zeroone.star.project.j1.menu.MenuApis;
import com.zeroone.star.project.query.menu.MenuDetailQuery;
import com.zeroone.star.project.query.menu.MenuListPageQuery;
import com.zeroone.star.project.dto.PageDTO;
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
@RequestMapping("/menu")
@Tag(name = "菜单管理")
public class MenuController implements MenuApis {

    /**
     * 加载菜单树结构
     * <p>
     *  注：这里要注意做登录校验，如果获取不到用户登录信息，直接返回空列表
     * </p>
     * @see com.zeroone.star.project.components.user.UserHolder#getCurrentUser()
     * @return 菜单树结构VO
     * @author 是丫丫
     */
    @Override
    @PostMapping("getTree")
    @Operation(summary = "获取菜单名称树")
    public JsonVO<MenuTreeVO> getMenuTree() {
        return JsonVO.success(null);
    }

    /**
     * 获取菜单列表（条件 + 分页）
     * <p>
     *  注：这里要注意做登录校验，如果获取不到用户登录信息，直接返回空列表
     * </p>
     * @see com.zeroone.star.project.components.user.UserHolder#getCurrentUser()
     * @param query 查询条件
     * @return 菜单分页列表
     * @author 是丫丫
     */
    @Override
    @PostMapping("getList")
    @Operation(summary = "获取菜单列表（条件 + 分页）")
    public JsonVO<PageDTO<MenuVO>> getMenuList(@RequestBody @Valid MenuListPageQuery query) {
        return JsonVO.success(null);
    }

    /**
     * 加载菜单详情信息
     * @param query 查询条件
     * @return 菜单详情信息VO
     * @author 是丫丫
     */
    @Override
    @PostMapping("getById")
    @Operation(summary = "获取指定菜单详情")
    public JsonVO<MenuVO> getMenu(@RequestBody @Valid MenuDetailQuery query) {
        return JsonVO.success(null);
    }

    /**
     * <p>
     *     根据当前登录用户来查询可见菜单树结构 <br>
     *     注：这里要注意做登录校验，如果获取不到用户登录信息，直接返回空列表
     * </p>
     * @see com.zeroone.star.project.components.user.UserHolder#getCurrentUser()
     * @return 菜单树结构VO
     */
    @Override
    @PostMapping("getTreeByLoginUser")
    @Operation(summary = "根据当前登录用户来查询可见菜单树结构")
    public JsonVO<MenuTreeVO> getTreeByLoginUser() {
        return JsonVO.success(null);
    }

    /**
     * 创建菜单信息
     * @param menuDto 菜单创建入参
     * @return 是否创建成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("create")
    @Operation(summary = "新增菜单")
    public JsonVO<Boolean> create(@RequestBody @Valid MenuCreateDTO menuDto) {
        return JsonVO.success(null);
    }

    /**
     * 更新菜单信息
     * @param menuDto 菜单修改入参
     * @return 是否修改成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("update")
    @Operation(summary = "修改菜单")
    public JsonVO<Boolean> update(@RequestBody @Valid MenuUpdateDTO menuDto) {
        return JsonVO.success(null);
    }

    /**
     * 删除菜单信息
     * @param menuDto 菜单删除入参
     * @return 是否删除成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("delete")
    @Operation(summary = "删除菜单")
    public JsonVO<Boolean> delete(@RequestBody @Valid MenuDeleteDTO menuDto) {
        return JsonVO.success(null);
    }

    /**
     * 修改菜单状态  启用/停用菜单
     * @param menuDto 菜单启用入参
     * @return 是否修改成功
     * @author 是丫丫
     */
    @Override
    @PostMapping("enable")
    @Operation(summary = "启用/禁用菜单")
    public JsonVO<Boolean> editStatus(@RequestBody @Valid MenuEditStatusDto menuDto) {
        return JsonVO.success(null);
    }
}
