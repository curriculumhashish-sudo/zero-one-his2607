package com.zeroone.star.sysmgr.controller.menu;

import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/auth")
@Tag(name = "菜单接口", description = "登录用户菜单权限树")
public class MenuController {

    @GetMapping("/menus")
    @Operation(summary = "获取菜单", description = "获取当前登录用户的菜单权限树")
    public JsonVO<?> getMenus() {
        return null;
    }
}
