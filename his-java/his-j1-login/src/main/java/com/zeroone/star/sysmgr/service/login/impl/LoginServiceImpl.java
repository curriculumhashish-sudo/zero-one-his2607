package com.zeroone.star.sysmgr.service.login.impl;

import cn.hutool.json.JSONArray;
import cn.hutool.json.JSONObject;
import com.zeroone.star.project.components.jwt.JwtComponent;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.constant.RedisConstant;
import com.zeroone.star.project.dto.login.LoginDTO;
import com.zeroone.star.project.dto.login.Oauth2TokenDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.login.LoginVO;
import com.zeroone.star.sysmgr.entity.login.LoginUserProfileDO;
import com.zeroone.star.sysmgr.mapper.login.LoginSessionMapper;
import com.zeroone.star.sysmgr.mapper.login.LoginUserMapper;
import com.zeroone.star.sysmgr.service.login.ILoginService;
import com.zeroone.star.sysmgr.service.login.OauthService;
import jakarta.annotation.Resource;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.time.Duration;
import java.util.HashMap;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class LoginServiceImpl implements ILoginService {

    private static final Logger log = LoggerFactory.getLogger(LoginServiceImpl.class);
    private static final String BEARER_PREFIX = "Bearer ";
    private static final Duration LOGIN_CONTEXT_TTL = Duration.ofHours(1);

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
    private LoginSessionMapper loginSessionMapper;

    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    @Resource
    private StringRedisTemplate stringRedisTemplate;

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
        writeLoginContext(userId, loginDTO.getUsername(), profile, tokenDTO);

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
            // 优先从统一登录上下文读取（包含身份 + 租户/医院 + 角色 + token）
            LoginVO vo = readLoginContext(userId);
            if (vo != null) {
                return JsonVO.success(vo);
            }
            // 上下文缺失时回退到数据库查询
            LoginUserProfileDO profile = loginUserMapper.selectProfileById(userId);
            if (profile == null) {
                return JsonVO.fail("用户不存在或已被删除");
            }
            return JsonVO.success(toLoginVO(profile));
        } catch (Exception exception) {
            return JsonVO.fail("登录状态无效：" + safeMessage(exception));
        }
    }

    // ---- 统一登录上下文（his:login:context:{userId}）----

    /**
     * 登录成功写入统一登录上下文（his:login:context:{userId}）。
     * Hash 字段：userId、username、name、email、phone、tenantId、hospitalId、token。TTL=1h。
     */
    private void writeLoginContext(String userId, String username, LoginUserProfileDO profile, Oauth2TokenDTO token) {
        if (!StringUtils.hasText(userId)) {
            return;
        }
        try {
            String key = RedisConstant.LOGIN_CONTEXT_PREFIX + userId;
            Map<String, String> ctx = new HashMap<>(8);
            ctx.put("userId", userId);
            ctx.put("username", username);
            if (token != null) {
                ctx.put("token", token.getToken());
            }
            if (profile != null) {
                ctx.put("name", profile.getName());
                ctx.put("email", profile.getEmail());
                ctx.put("phone", profile.getPhoneNumber());
                ctx.put("tenantId", profile.getTenantId());
                ctx.put("hospitalId", profile.getHospitalOrganizationId());
            }
            stringRedisTemplate.opsForHash().putAll(key, ctx);
            stringRedisTemplate.expire(key, LOGIN_CONTEXT_TTL);
        } catch (Exception e) {
            log.warn("写登录上下文到 Redis 失败: {}", e.getMessage());
        }
    }

    /**
     * 读取统一登录上下文并映射为 LoginVO。上下文缺失或异常时返回 null。
     */
    private LoginVO readLoginContext(String userId) {
        try {
            String key = RedisConstant.LOGIN_CONTEXT_PREFIX + userId;
            Map<Object, Object> ctx = stringRedisTemplate.opsForHash().entries(key);
            if (ctx == null || ctx.isEmpty()) {
                return null;
            }
            LoginVO vo = new LoginVO();
            vo.setId(toString(ctx.get("userId")));
            vo.setUsername(toString(ctx.get("username")));
            vo.setName(toString(ctx.get("name")));
            vo.setEmail(toString(ctx.get("email")));
            vo.setPhoneNumber(toString(ctx.get("phone")));
            vo.setTenantId(toString(ctx.get("tenantId")));
            vo.setHospitalOrganizationId(toString(ctx.get("hospitalId")));
            return vo;
        } catch (Exception e) {
            log.warn("从登录上下文读取失败，回退数据库: {}", e.getMessage());
            return null;
        }
    }

    private static String toString(Object value) {
        return value == null ? null : value.toString();
    }

    /**
     * 退出登录时删除统一登录上下文。
     */
    private void deleteLoginContext() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                // 必须用 StringRedisTemplate 删除：写入侧同样使用字符串序列化，
                // 否则 RedisTemplate 默认 JDK 键序列化会导致 key 不匹配、删除无效。
                stringRedisTemplate.delete(RedisConstant.LOGIN_CONTEXT_PREFIX + currentUser.getId());
            }
        } catch (Exception e) {
            log.warn("退出登录时清除登录上下文失败: {}", e.getMessage());
        }
    }

    // ---- JWT 解析工具 ----

    private JSONObject decodeToken(Oauth2TokenDTO token) {
        if (token == null || !StringUtils.hasText(token.getToken())) {
            return null;
        }
        try {
            return new JSONObject(jwtComponent.defaultRsaVerify(token.getToken()));
        } catch (Exception e) {
            log.warn("解析登录 JWT 失败: {}", e.getMessage());
            return null;
        }
    }

    private String joinRoles(JSONObject claims) {
        Object roles = claims.get("roles");
        if (roles instanceof JSONArray arr) {
            return arr.toList(String.class).stream().collect(Collectors.joining(","));
        }
        return roles == null ? "" : roles.toString();
    }

    private String joinPermissions(JSONObject claims) {
        Object perms = claims.get("permissions");
        if (perms instanceof JSONArray arr) {
            return arr.toList(String.class).stream().collect(Collectors.joining(","));
        }
        return "";
    }

    // ---- 工具方法 ----

    private String asString(Map<String, String> map, String key) {
        return map == null ? null : map.get(key);
    }

    private Byte asByte(Map<Object, Object> map, String key) {
        Object value = map == null ? null : map.get(key);
        if (value == null) {
            return null;
        }
        if (value instanceof Number number) {
            return number.byteValue();
        }
        return Byte.valueOf(value.toString());
    }

    private Boolean asBoolean(Map<Object, Object> map, String key) {
        Object value = map == null ? null : map.get(key);
        if (value == null) {
            return null;
        }
        if (value instanceof Boolean b) {
            return b;
        }
        return Boolean.valueOf(value.toString());
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

    private void touchLoginSession(String username) {
        try {
            String userId = loginUserMapper.selectIdByUsername(username);
            if (StringUtils.hasText(userId)) {
                loginSessionMapper.touchByUserId(userId);
            }
        } catch (Exception ignored) {
        }
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
