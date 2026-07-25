package com.zeroone.star.sysmgr.controller.login;

import com.zeroone.star.project.dto.login.LoginDTO;
import com.zeroone.star.project.dto.login.Oauth2TokenDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;
import com.zeroone.star.sysmgr.service.login.ILoginService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping
@Tag(name = "登录认证", description = "J1组登录认证第一组接口：账号登录、退出登录、当前用户信息")
public class LoginController {

    @Resource
    ILoginService loginService;

    @PostMapping("/login")
    @Operation(summary = "账号登录", description = "通过用户名、密码和验证码进行身份认证，成功后返回OAuth2访问令牌")
    public JsonVO<Oauth2TokenDTO> authLogin(@Validated @RequestBody LoginDTO loginDTO) {
        return loginService.login(loginDTO);
    }

    @PostMapping("/logout")
    @Operation(summary = "退出登录", description = "注销当前用户会话，清除服务器端的Token认证状态")
    public JsonVO<String> logout() {
        return loginService.logout();
    }

    @Operation(summary = "获取当前用户")
    @GetMapping("/user/info")
    public JsonVO<LoginVO> getCurrUser() {
        return loginService.getCurrentUser();
    }
}
