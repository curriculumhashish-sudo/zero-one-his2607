package com.zeroone.star.sysmgr.controller.menu;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.menu.*;
import com.zeroone.star.project.j1.menu.MenuApis;
import com.zeroone.star.project.query.menu.MenuDetailQuery;
import com.zeroone.star.project.query.menu.MenuListPageQuery;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.menu.MenuVO;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.sysmgr.exception.MenuStatus;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.service.menu.MenusService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import lombok.RequiredArgsConstructor;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * 菜单管理的api接口
 * @author 是丫丫
 * @date  2026-07-19
 */
@RestController
@RequestMapping("/menu")
@Tag(name = "菜单管理")
@RequiredArgsConstructor
public class MenuController implements MenuApis {

    private final MenusService menusService;
    private final UserHolder userHolder;

    /**
     * 加载菜单树结构
     * <p>
     *  注：如果获取不到用户登录信息，属于非法请求，抛出异常
     * </p>
     * @see com.zeroone.star.project.components.user.UserHolder#getCurrentUser()
     * @return 菜单树结构VO
     * @author 是丫丫
     * @负责人 倾墨
     */
    @Override
    @PostMapping("getTree")
    @Operation(summary = "获取菜单名称树")
    public JsonVO<List<MenuTreeVO>> getMenuTree() throws Exception {
        // user认证方面的异常由getCurrentUser内部处理,这里直接抛出异常给全局异常处理器处理。
        UserDTO user = userHolder.getCurrentUser();
        // 获取到的dto参数为null或者角色列表为null,抛出异常
        if(user == null || user.getRoles()==null){
            throw new SystemException(MenuStatus.USER_INFO_MISSING);
        }else {
            List<String> roles = user.getRoles();
            List<MenuTreeVO> menuTree = menusService.getMenuTree(roles);
            return JsonVO.success(menuTree);
        }
    }

    /**
     * 获取菜单列表（条件 + 分页）
     * <p>
     *  注：如果获取不到用户登录信息，属于非法请求，抛出异常
     * </p>
     * @see com.zeroone.star.project.components.user.UserHolder#getCurrentUser()
     * @param query 查询条件
     * @return 菜单分页列表
     * @author 是丫丫
     * @负责人 倾墨
     */
    @Override
    @PostMapping("getList")
    @Operation(summary = "获取菜单列表（条件 + 分页）")
    public JsonVO<PageDTO<MenuVO>> getMenuList(@RequestBody @Valid MenuListPageQuery query) throws Exception {
        // user认证方面的异常由getCurrentUser内部处理,这里直接抛出异常给全局异常处理器处理。
        UserDTO user = userHolder.getCurrentUser();
        // 获取到的dto参数为null或者角色列表为null,抛出异常
        if(user == null || user.getRoles()==null){
            throw new SystemException(MenuStatus.USER_INFO_MISSING);
        }
        return JsonVO.success(menusService.getMenuList(user.getRoles(), query));
    }

    /**
     * 加载菜单详情信息
     * @param query 查询条件
     * @return 菜单详情信息VO
     * @author 是丫丫
     * @负责人 Drums小白
     */
    @Override
    @PostMapping("getById")
    @Operation(summary = "获取指定菜单详情")
    public JsonVO<MenuVO> getMenu(@RequestBody @Valid MenuDetailQuery query) {
        String menuId = query.getId();
        MenuVO menuVO = menusService.getMenuDetail(menuId);
        return JsonVO.success(menuVO);
    }

    /**
     * 创建菜单信息
     * @param menuDto 菜单创建入参
     * @return 是否创建成功
     * @author 是丫丫
     * @负责人 是丫丫
     */
    @Override
    @PostMapping("create")
    @Operation(summary = "新增菜单")
    public JsonVO<Boolean> create(@RequestBody @Valid MenuCreateDTO menuDto) {
        return JsonVO.success(menusService.create(menuDto));
    }

    /**
     * 更新菜单信息
     * @param menuDto 菜单修改入参
     * @return 是否修改成功
     * @author 是丫丫
     * @负责人 Drums小白
     */
    @Override
    @PutMapping("update")
    @Operation(summary = "修改菜单基础信息")
    public JsonVO<Boolean> update(@RequestBody @Valid MenuUpdateDTO menuDto) {
        Boolean success = menusService.editMenu(menuDto);
        return JsonVO.success(success);
    }

    /**
     * 删除菜单
     * @author 是丫丫
     * @负责人 Drums小白
     * @param menuDto 删除参数
     * @return 删除结果
     */
    @Override
    @DeleteMapping("/delete")
    @Operation(summary = "删除菜单")
    public JsonVO<Boolean> delete(@RequestBody @Valid MenuDeleteDTO menuDto) {
        return menusService.delete(menuDto);
    }

    /**
     * 修改菜单状态  启用/停用菜单
     * @param menuDto 菜单启用入参
     * @return 是否修改成功
     * @author 是丫丫
     * @负责人 是丫丫
     */
    @Override
    @PostMapping("enable")
    @Operation(summary = "启用/禁用菜单")
    public JsonVO<Boolean> editStatus(@RequestBody @Valid MenuEditStatusDTO menuDto) {
        return JsonVO.success(menusService.editStatus(menuDto));
    }
}
