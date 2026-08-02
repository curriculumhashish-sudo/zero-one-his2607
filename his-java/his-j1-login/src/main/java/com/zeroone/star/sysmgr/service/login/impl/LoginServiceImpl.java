package com.zeroone.star.sysmgr.service.login.impl;

import cn.hutool.json.JSONObject;
import cn.hutool.json.JSONArray;
import com.zeroone.star.project.components.jwt.JwtComponent;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.login.LoginDTO;
import com.zeroone.star.project.dto.login.Oauth2TokenDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;
import com.zeroone.star.sysmgr.entity.login.LoginUserProfileDO;
import com.zeroone.star.sysmgr.mapper.login.LoginUserMapper;
import com.zeroone.star.sysmgr.service.login.ILoginService;
import com.zeroone.star.sysmgr.service.login.LoginContextService;
import com.zeroone.star.sysmgr.service.login.OauthService;
import jakarta.annotation.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.util.*;

@Service
public class LoginServiceImpl implements ILoginService {

    private static final Logger log = LoggerFactory.getLogger(LoginServiceImpl.class);
    private static final String BEARER_PREFIX = "Bearer ";
    /** 前端契约字段：无数据来源时的默认首页路径 */
    private static final String DEFAULT_HOME_PATH = "/dashboard";

    @Value("${zo.oauth2.mgr-id:project-manager}")
    private String clientId;

    @Value("${zo.oauth2.mgr-secret:WarfhT2N_BY_NY9JJTSTgNAZYgaltrVSbfLisHZhYAQ}")
    private String clientSecret;

    @Resource
    private OauthService oauthService;

    @Resource
    private UserHolder userHolder;

    @Resource
    private JwtComponent jwtComponent;

    @Resource
    private LoginUserMapper loginUserMapper;

    @Resource
    private LoginContextService loginContextService;

    @Override
    public JsonVO<Oauth2TokenDTO> login(LoginDTO loginDTO) {
        if (loginDTO == null || !StringUtils.hasText(loginDTO.getUsername())
                || !StringUtils.hasText(loginDTO.getPassword())) {
            return JsonVO.fail("用户名和密码不能为空");
        }

        // 第 1 步：组装 OAuth2 /oauth2/token 表单参数
        Map<String, String> parameters = new HashMap<>(5);
        parameters.put("grant_type", "password");
        parameters.put("client_id", clientId);
        parameters.put("client_secret", clientSecret);
        parameters.put("username", loginDTO.getUsername());
        parameters.put("password", loginDTO.getPassword());

        // 第 2 步：调用 OAuth2 认证服务
        Map<String, Object> response = oauthService.postAccessToken(parameters);

        // 第 3 步：将返回结果转换为 Oauth2TokenDTO
        String error = getErrorMessage(response);
        if (error != null) {
            return JsonVO.fail(error);
        }
        Oauth2TokenDTO tokenDTO = toTokenDTO(response);
        if (!StringUtils.hasText(tokenDTO.getToken())) {
            return JsonVO.fail("认证服务未返回访问令牌");
        }

        // 第 4 步：登录成功后写入统一登录上下文（his:login:context:{userId}）
        String userId = loginUserMapper.selectIdByUsername(loginDTO.getUsername());
        LoginUserProfileDO profile = StringUtils.hasText(userId)
                ? loginUserMapper.selectProfileById(userId) : null;
        if (!StringUtils.hasText(userId)) {
            return JsonVO.fail("认证成功但未找到用户资料");
        }
        writeLoginContext(userId, profile, tokenDTO.getToken());

        // 第 5 步：返回成功结果
        return JsonVO.success(tokenDTO);
    }

    @Override
    public JsonVO<String> logout() {
        // 第 1 步：从当前请求获取 token
        String token;
        try {
            token = userHolder.getCurrentToken();
        } catch (Exception exception) {
            return JsonVO.fail("无法读取当前登录凭证");
        }
        if (!StringUtils.hasText(token)) {
            return JsonVO.fail("未登录或访问令牌不存在");
        }

        // 第 2 步：调用 OAuth2 撤销接口，使服务端令牌失效
        try {
            oauthService.revokeToken(Map.of(
                    "token", token,
                    "client_id", clientId,
                    "client_secret", clientSecret
            ));
        } catch (Exception exception) {
            log.warn("调用 OAuth2 撤销接口失败（忽略，继续清理本地上下文）: {}", safeMessage(exception));
        }

        // 第 3 步：删除 Redis 中的统一登录上下文 his:login:context:{userId}
        deleteLoginContext();

        // 第 4 步：返回成功
        return JsonVO.success("退出成功");
    }

    @Override
    public JsonVO<LoginVO> getCurrentUser() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                return JsonVO.fail("用户未登录");
            }
            String userId = currentUser.getId();
            String currentToken = userHolder.getCurrentToken();
            // 优先从统一登录上下文读取（包含身份 + 租户/医院 + 角色 + token）
            LoginVO vo = readLoginContext(userId);
            if (vo != null) {
                applyFrontendFields(vo, currentToken);
                loginContextService.refreshContext(userId);
                return JsonVO.success(vo);
            }
            // context 不存在时先读取 user:info。它只保存设计规定的基础资料，
            // 为保证 LoginVO 和数据库路径字段完全一致，仍以 DB 查询补齐会话/医护字段。
            Map<String, String> userInfo = loginContextService.loadUserInfo(userId);
            LoginUserProfileDO profile = loginUserMapper.selectProfileById(userId);
            if (profile == null) {
                return JsonVO.fail("用户不存在或已被删除");
            }
            LoginVO result = toLoginVO(profile);
            if (!userInfo.isEmpty()) {
                applyUserInfo(result, userInfo);
            }
            // MySQL 回源后同时回填两类缓存，确保下次请求可命中完整上下文。
            writeLoginContext(userId, profile, userHolder.getCurrentToken());
            applyFrontendFields(result, currentToken);
            return JsonVO.success(result);
        } catch (Exception exception) {
            return JsonVO.fail("登录状态无效：" + safeMessage(exception));
        }
    }

    /**
     * 填充前端契约要求、但 LoginVO 原有字段未覆盖的展示字段。
     * 其中 avatar/desc 数据库当前无数据来源，返回空串；homePath 使用默认首页路径。
     */
    private void applyFrontendFields(LoginVO vo, String currentToken) {
        if (vo == null) {
            return;
        }
        vo.setUserId(vo.getId());
        vo.setRealName(vo.getName());
        if (!StringUtils.hasText(vo.getToken())) {
            vo.setToken(currentToken);
        }
        vo.setAvatar(vo.getAvatar() == null ? "" : vo.getAvatar());
        vo.setDesc(vo.getDesc() == null ? "" : vo.getDesc());
        vo.setHomePath(StringUtils.hasText(vo.getHomePath()) ? vo.getHomePath() : DEFAULT_HOME_PATH);
    }

    private void writeLoginContext(String userId, LoginUserProfileDO profile, String token) {
        try {
            JSONObject claims = decodeToken(token);
            JSONArray rolesJson = claims == null ? null : claims.getJSONArray("roles");
            JSONArray permissionsJson = claims == null ? null : claims.getJSONArray("permissions");
            List<String> roles = rolesJson == null ? List.of() : rolesJson.toList(String.class);
            String permissions = permissionsJson == null ? "[]" : permissionsJson.toString();
            loginContextService.saveContext(userId, token, profile, roles, permissions);
            loginContextService.saveUserInfo(userId, profile);
        } catch (Exception e) {
            log.warn("写登录上下文到 Redis 失败: {}", e.getMessage());
        }
    }

    /**
     * 读取统一登录上下文并映射为 LoginVO。上下文缺失或异常时返回 null。
     */
    private LoginVO readLoginContext(String userId) {
        try {
            Map<String, String> ctx = loginContextService.loadContext(userId);
            if (ctx.isEmpty()) {
                return null;
            }
            LoginVO vo = new LoginVO();
            vo.setId(ctx.get("userId"));
            vo.setUsername(ctx.get("username"));
            vo.setName(ctx.get("realName"));
            vo.setToken(ctx.get("token"));
            vo.setEmail(ctx.get("email"));
            vo.setPhoneNumber(ctx.get("phone_number"));
            vo.setIsEnabled(toByte(ctx.get("is_enabled")));
            vo.setLockedOut(toBoolean(ctx.get("is_locked_out")));
            vo.setTenantId(ctx.get("tenantId"));
            vo.setHospitalOrganizationId(ctx.get("hospitalId"));
            vo.setOrganizationUnitId(ctx.get("deptId"));
            vo.setPractitionerId(ctx.get("practitionerId"));
            vo.setPractitionerName(ctx.get("practitionerName"));
            vo.setPractitionerStatus(ctx.get("practitionerStatus"));
            return vo;
        } catch (Exception e) {
            log.warn("从登录上下文读取失败，回退数据库: {}", e.getMessage());
            return null;
        }
    }

    /**
     * 退出登录时删除统一登录上下文。
     */
    private void deleteLoginContext() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                loginContextService.removeContext(currentUser.getId());
                loginContextService.removeUserInfo(currentUser.getId());
            }
        } catch (Exception e) {
            log.warn("退出登录时清除登录上下文失败: {}", e.getMessage());
        }
    }

    // ---- JWT 解析工具 ----

    private JSONObject decodeToken(String token) {
        if (!StringUtils.hasText(token)) {
            return null;
        }
        try {
            return new JSONObject(jwtComponent.defaultRsaVerify(token));
        } catch (Exception e) {
            log.warn("解析登录 JWT 失败: {}", e.getMessage());
            return null;
        }
    }

    // ---- 工具方法 ----

    private Byte toByte(String value) {
        return StringUtils.hasText(value) ? Byte.valueOf(value) : null;
    }

    private Boolean toBoolean(String value) {
        return StringUtils.hasText(value) ? Boolean.valueOf(value) : null;
    }

    private void applyUserInfo(LoginVO vo, Map<String, String> userInfo) {
        vo.setId(userInfo.get("id"));
        vo.setUsername(userInfo.get("username"));
        vo.setName(userInfo.get("name"));
        vo.setEmail(userInfo.get("email"));
        vo.setPhoneNumber(userInfo.get("phone_number"));
        vo.setIsEnabled(toByte(userInfo.get("is_enabled")));
        vo.setLockedOut(toBoolean(userInfo.get("is_locked_out")));
    }

    private Oauth2TokenDTO toTokenDTO(Map<String, Object> response) {
        Oauth2TokenDTO token = new Oauth2TokenDTO();
        token.setToken(stringVal(response, "access_token"));
        token.setRefreshToken(stringVal(response, "refresh_token"));
        token.setTokenHead(BEARER_PREFIX);
        Object expiresIn = response == null ? null : response.get("expires_in");
        if (expiresIn instanceof Number number) {
            token.setExpiresIn(number.intValue());
        } else if (expiresIn != null) {
            try {
                token.setExpiresIn(Integer.valueOf(expiresIn.toString()));
            } catch (NumberFormatException ignored) {
            }
        }
        return token;
    }

    private String getErrorMessage(Map<String, Object> response) {
        if (response == null) {
            return "认证服务未返回响应";
        }
        Object error = response.get(OauthService.ERROR_KEY);
        if (error == null) {
            return null;
        }
        Object description = response.get("error_description");
        return description == null ? error.toString() : description.toString();
    }

    private String stringVal(Map<String, Object> response, String key) {
        Object value = response == null ? null : response.get(key);
        return value == null ? null : value.toString();
    }

    private String safeMessage(Exception exception) {
        return StringUtils.hasText(exception.getMessage()) ? exception.getMessage() : "未知错误";
    }

    private LoginVO toLoginVO(LoginUserProfileDO profile) {
        LoginVO result = new LoginVO();
        result.setId(profile.getId());
        result.setUsername(profile.getUsername());
        result.setName(profile.getName());
        result.setEmail(profile.getEmail());
        result.setPhoneNumber(profile.getPhoneNumber());
        result.setIsEnabled(profile.getIsEnabled());
        result.setLockedOut(profile.getLockedOut());
        result.setTenantId(profile.getTenantId());
        result.setHospitalOrganizationId(profile.getHospitalOrganizationId());
        result.setOrganizationUnitId(profile.getOrganizationUnitId());
        result.setPractitionerId(profile.getPractitionerId());
        result.setPractitionerName(profile.getPractitionerName());
        result.setPractitionerStatus(profile.getPractitionerStatus());
        return result;
    }
}
