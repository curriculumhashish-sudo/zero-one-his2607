package com.zeroone.star.sysmgr.controller.login;

import com.zeroone.star.project.dto.login.LoginDTO;
import com.zeroone.star.project.dto.login.Oauth2TokenDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

/**
 * <p>
 * 描述：J1组登录认证接口控制器（第一组）
 * </p>
 * <p>
 * 说明：账号登录、退出登录、当前用户信息。
 * 账号登录通过 Feign 调用 his-oauth2 模块的 OAuth2 认证服务端点。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author J1组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/auth")
@Tag(name = "登录认证", description = "J1组登录认证第一组接口：账号登录、退出登录、当前用户信息")
public class LoginController {

    /**
     * 客户端 ID（管理端）
     * <p>TODO: 建议从配置文件或 Nacos 配置中读取，避免硬编码</p>
     */
    private static final String CLIENT_ID = "project-manager";

    // ========================= 1. 账号登录 =========================

    @PostMapping("/login")
    @Operation(summary = "账号登录", description = "通过用户名、密码和验证码进行身份认证，成功后返回OAuth2访问令牌")
    public JsonVO<Oauth2TokenDTO> authLogin(@Validated @RequestBody LoginDTO loginDTO) {
        return null;
    }

    // ========================= 2. 退出登录 =========================

    @PostMapping("/logout")
    @Operation(summary = "退出登录", description = "注销当前用户会话，清除服务器端的Token认证状态")
    public JsonVO<String> logout() {
        return null;
    }

    // ========================= 3. 当前用户信息 =========================

    @GetMapping("/user/info")
    @Operation(summary = "当前用户信息", description = "获取当前登录用户的详细信息")
    public JsonVO<LoginVO> getCurrUser() {
        return null;
    }

}
