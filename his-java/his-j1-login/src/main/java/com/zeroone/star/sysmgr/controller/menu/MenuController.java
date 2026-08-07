package com.zeroone.star.sysmgr.controller.menu;

import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@Slf4j
@RestController
@RequestMapping("/auth")
@Tag(name = "菜单接口", description = "登录用户菜单权限树")
public class MenuController {

    @Resource
    private UserHolder userHolder;

    @GetMapping("/menus")
    @Operation(summary = "获取菜单", description = "获取当前登录用户的菜单权限树")
    public JsonVO<?> getMenus() {
        String userId = getCurrentUserId();
        if (userId == null) {
            return JsonVO.fail("未登录");
        }
        // TODO: 接入菜单服务后替换为实际实现
        return JsonVO.fail("功能尚未实现");
    }

    /**
     * 从请求上下文获取当前登录用户ID，未登录返回null
     */
    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user != null ? user.getId() : null;
        } catch (Exception e) {
            log.warn("获取当前用户失败: {}", e.getMessage());
            return null;
        }
    }
}
