package com.zeroone.star.sysmgr.controller;

import cn.hutool.core.bean.BeanUtil;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j1.login.LoginDTO;
import com.zeroone.star.project.dto.j1.login.Oauth2TokenDTO;
import com.zeroone.star.project.dto.j1.login.SelectHospitalDTO;
import com.zeroone.star.project.dto.j1.login.SelectTenantDTO;
import com.zeroone.star.project.enums.ResultStatus;
import com.zeroone.star.project.j1.login.LoginApi;
import com.zeroone.star.project.query.j1.login.HospitalQuery;
import com.zeroone.star.project.query.j1.login.TenantQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j1.login.HospitalVO;
import com.zeroone.star.project.vo.j1.login.LoginVO;
import com.zeroone.star.project.vo.j1.login.SelectResultVO;
import com.zeroone.star.project.vo.j1.login.TenantVO;
import com.zeroone.star.sysmgr.service.OauthService;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;

/**
 * <p>
 * 描述：J1组登录认证接口控制器（第一组）
 * </p>
 * <p>
 * 说明：实现 {@link LoginApi} 接口第一组功能，包含：账号登录、退出登录、当前用户信息、租户选择、医院选择。
 * 账号登录通过 Feign 调用 his-oauth2 模块的 OAuth2 认证服务端点。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author J1组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/auth")
@Tag(name = "登录认证", description = "J1组登录认证接口：账号登录、退出登录、当前用户信息、租户医院选择")
public class LoginController implements LoginApi, LoginApi.LoginApis {


    @Resource
    UserHolder userHolder;

    @Resource
    OauthService oauthService;


    /**
     * 客户端 ID（管理端）
     * <p>TODO: 建议从配置文件或 Nacos 配置中读取，避免硬编码</p>
     */
    private static final String CLIENT_ID = "project-manager";


    // ========================= 1. 账号登录 =========================


    @Override
    @PostMapping("/login")
    @Operation(summary = "账号登录", description = "通过用户名、密码和验证码进行身份认证，成功后返回OAuth2访问令牌")
    public JsonVO<Oauth2TokenDTO> authLogin(@Validated @RequestBody LoginDTO loginDTO) {

        Map<String, String> params = Map.of(
                "grant_type", "password",
                "username", loginDTO.getUserName(),
                "password", loginDTO.getPassword(),
                "client_id", CLIENT_ID,
                "scope", "openid profile"
        );

        Map<String, Object> result = oauthService.postAccessToken(params);

        if (result.containsKey(OauthService.ERROR_KEY)) {
            return JsonVO.fail(result.get(OauthService.ERROR_KEY).toString());
        }

        Oauth2TokenDTO tokenDTO = mapToTokenDTO(result);

        return JsonVO.success(tokenDTO);
    }



    // ========================= 2. 退出登录 =========================


    @Override
    @PostMapping("/logout")
    @Operation(summary = "退出登录", description = "注销当前用户会话，清除服务器端的Token认证状态")
    public JsonVO<String> logout() {

        String token = userHolder.getCurrentToken();

        if (token == null) {
            return JsonVO.fail("未获取到Token，退出失败");
        }

        Map<String, String> params = Map.of(
                "token", token,
                "client_id", CLIENT_ID
        );


        Map<String, Object> result = oauthService.revokeToken(params);

        if (result.containsKey(OauthService.ERROR_KEY)) {
            return JsonVO.fail(result.get(OauthService.ERROR_KEY).toString());
        }

        return JsonVO.success("退出成功");
    }



    // ========================= 3. 当前用户信息 =========================


    @Override
    @GetMapping("/user/info")
    @Operation(summary = "当前用户信息", description = "获取当前登录用户的详细信息")
    public JsonVO<LoginVO> getCurrUser() {

        UserDTO currentUser;

        try {

            currentUser = userHolder.getCurrentUser();

        } catch (Exception e) {

            return JsonVO.create(null, ResultStatus.FAIL.getCode(), e.getMessage());

        }


        if (currentUser == null) {

            return JsonVO.fail(null);

        } else {

            LoginVO vo = new LoginVO();

            BeanUtil.copyProperties(currentUser, vo);

            return JsonVO.success(vo);

        }

    }



    // ========================= 4. 租户医院选择 =========================


    @Override
    public JsonVO<List<TenantVO>> getTenantList(TenantQuery query) {
        return null;
    }


    @Override
    public JsonVO<SelectResultVO> selectTenant(SelectTenantDTO dto) {
        return null;
    }


    @Override
    public JsonVO<List<HospitalVO>> getHospitalList(HospitalQuery query) {
        return null;
    }


    @Override
    public JsonVO<SelectResultVO> selectHospital(SelectHospitalDTO dto) {
        return null;
    }




    // ========================= 辅助方法 =========================


    private Oauth2TokenDTO mapToTokenDTO(Map<String, Object> map) {

        Oauth2TokenDTO dto = new Oauth2TokenDTO();

        dto.setToken(getStringValue(map, "access_token"));

        dto.setRefreshToken(getStringValue(map, "refresh_token"));

        String tokenType = getStringValue(map, "token_type");

        dto.setTokenHead(tokenType != null ? tokenType + " " : "Bearer ");

        dto.setExpiresIn(getIntValue(map, "expires_in"));

        return dto;

    }



    private String getStringValue(Map<String, Object> map, String key) {

        Object value = map.get(key);

        return value != null ? value.toString() : null;

    }



    private Integer getIntValue(Map<String, Object> map, String key) {

        Object value = map.get(key);

        if (value instanceof Number) {

            return ((Number) value).intValue();

        }

        return null;

    }

}