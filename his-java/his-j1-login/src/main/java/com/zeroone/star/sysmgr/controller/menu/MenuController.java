package com.zeroone.star.sysmgr.controller.menu;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.MenuTreeVO;
import com.zeroone.star.sysmgr.service.menu.MenuService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@Slf4j
@RestController
@RequestMapping("/auth")
@Tag(name = "菜单接口", description = "登录用户菜单权限树")
public class MenuController {

    @Resource
    private UserHolder userHolder;

    @Resource
    private MenuService menuService;

    @GetMapping("/menus")
    @Operation(summary = "获取菜单", description = "获取当前登录用户的菜单权限树")
    public JsonVO<List<MenuTreeVO>> getMenus() {
        UserDTO user = getCurrentUser();
        if (user == null) {
            return JsonVO.fail("未登录");
        }
        List<String> roles = user.getRoles();
        if (roles == null || roles.isEmpty()) {
            return JsonVO.fail("当前用户无角色权限");
        }
        List<MenuTreeVO> tree = menuService.getMenuTree(roles);
        return JsonVO.success(tree);
    }

    /**
     * 从请求上下文获取当前登录用户，未登录返回null
     */
    private UserDTO getCurrentUser() {
        try {
            return userHolder.getCurrentUser();
        } catch (Exception e) {
            log.warn("获取当前用户失败: {}", e.getMessage());
            return null;
        }
    }
}
