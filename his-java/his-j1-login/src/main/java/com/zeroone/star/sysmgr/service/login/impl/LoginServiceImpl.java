package com.zeroone.star.sysmgr.service.login.impl;

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
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

@Service
public class LoginServiceImpl implements ILoginService {

    private static final Logger log = LoggerFactory.getLogger(LoginServiceImpl.class);
    private static final String BEARER_PREFIX = "Bearer ";
    private static final Duration USER_INFO_TTL = Duration.ofHours(1);
    private static final Duration USER_SESSION_TTL = Duration.ofHours(10);

    @Value("${zo.oauth2.mgr-id:project-manager}")
    private String clientId;

    @Resource
    private OauthService oauthService;

    @Resource
    private UserHolder userHolder;

    @Resource
    private LoginUserMapper loginUserMapper;

    @Resource
    private LoginSessionMapper loginSessionMapper;

    @Resource
    private RedisTemplate<String, Object> redisTemplate;

    @Override
    public JsonVO<Oauth2TokenDTO> login(LoginDTO loginDTO) {
        if (loginDTO == null || !StringUtils.hasText(loginDTO.getUsername())
                || !StringUtils.hasText(loginDTO.getPassword())) {
            return JsonVO.fail("用户名和密码不能为空");
        }

        Map<String, String> parameters = new HashMap<>(4);
        parameters.put("grant_type", "password");
        parameters.put("username", loginDTO.getUsername());
        parameters.put("password", loginDTO.getPassword());
        parameters.put("client_id", clientId);
        if (StringUtils.hasText(loginDTO.getCode())) {
            parameters.put("code", loginDTO.getCode());
        }

        try {
            Map<String, Object> response = oauthService.postAccessToken(parameters);
            String error = getErrorMessage(response);
            if (error != null) {
                return JsonVO.fail(error);
            }
            Oauth2TokenDTO token = toTokenDTO(response);
            if (!StringUtils.hasText(token.getToken())) {
                return JsonVO.fail("认证服务未返回访问令牌");
            }
            touchLoginSession(loginDTO.getUsername());
            cacheUserProfile(loginDTO.getUsername());
            return JsonVO.success(token);
        } catch (Exception exception) {
            return JsonVO.fail("认证服务调用失败：" + safeMessage(exception));
        }
    }

    @Override
    public JsonVO<String> logout() {
        String token;
        try {
            token = userHolder.getCurrentToken();
        } catch (Exception exception) {
            return JsonVO.fail("无法读取当前登录凭证");
        }
        if (!StringUtils.hasText(token)) {
            return JsonVO.fail("未登录或访问令牌不存在");
        }

        try {
            Map<String, Object> response = oauthService.revokeToken(Map.of(
                    "token", token,
                    "client_id", clientId
            ));
            String error = getErrorMessage(response);
            if (error == null) {
                deleteUserCache();
            }
            return error == null ? JsonVO.success("退出成功") : JsonVO.fail(error);
        } catch (Exception exception) {
            return JsonVO.fail("认证服务调用失败：" + safeMessage(exception));
        }
    }

    @Override
    public JsonVO<LoginVO> getCurrentUser() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                return JsonVO.fail("用户未登录");
            }
            LoginVO cached = loginVOFromCache(currentUser.getId());
            if (cached != null) {
                return JsonVO.success(cached);
            }
            LoginUserProfileDO profile = loginUserMapper.selectProfileById(currentUser.getId());
            if (profile == null) {
                return JsonVO.fail("用户不存在或已被删除");
            }
            LoginVO result = toLoginVO(profile);
            writeUserCache(currentUser.getId(), result);
            return JsonVO.success(result);
        } catch (Exception exception) {
            return JsonVO.fail("登录状态无效：" + safeMessage(exception));
        }
    }

    // ---- Redis 缓存 ----

    private void cacheUserProfile(String username) {
        try {
            String userId = loginUserMapper.selectIdByUsername(username);
            if (!StringUtils.hasText(userId)) {
                return;
            }
            LoginUserProfileDO profile = loginUserMapper.selectProfileById(userId);
            if (profile == null) {
                return;
            }
            writeUserCache(userId, toLoginVO(profile));
        } catch (Exception e) {
            log.warn("登录后缓存用户资料到 Redis 失败: {}", e.getMessage());
        }
    }

    private void writeUserCache(String userId, LoginVO vo) {
        String infoKey = RedisConstant.USER_INFO_PREFIX + userId;
        Map<String, Object> info = new HashMap<>();
        info.put("id", vo.getId());
        info.put("username", vo.getUsername());
        info.put("name", vo.getName());
        info.put("email", vo.getEmail());
        info.put("phone_number", vo.getPhoneNumber());
        info.put("is_enabled", vo.getIsEnabled());
        info.put("is_locked_out", vo.getLockedOut());
        try {
            redisTemplate.opsForHash().putAll(infoKey, info);
            redisTemplate.expire(infoKey, USER_INFO_TTL);
        } catch (Exception e) {
            log.warn("写用户信息缓存失败: {}", e.getMessage());
        }

        String sessionKey = RedisConstant.USER_SESSION_PREFIX + userId;
        Map<String, Object> session = new HashMap<>();
        session.put("tenant_id", vo.getTenantId());
        session.put("hospital_organization_id", vo.getHospitalOrganizationId());
        session.put("organization_unit_id", vo.getOrganizationUnitId());
        session.put("practitioner_id", vo.getPractitionerId());
        session.put("practitioner_name", vo.getPractitionerName());
        session.put("practitioner_status", vo.getPractitionerStatus());
        try {
            redisTemplate.opsForHash().putAll(sessionKey, session);
            redisTemplate.expire(sessionKey, USER_SESSION_TTL);
        } catch (Exception e) {
            log.warn("写用户会话缓存失败: {}", e.getMessage());
        }
    }

    private LoginVO loginVOFromCache(String userId) {
        try {
            String infoKey = RedisConstant.USER_INFO_PREFIX + userId;
            Map<Object, Object> info = redisTemplate.opsForHash().entries(infoKey);
            if (info.isEmpty()) {
                return null;
            }
            String sessionKey = RedisConstant.USER_SESSION_PREFIX + userId;
            Map<Object, Object> session = redisTemplate.opsForHash().entries(sessionKey);
            return fromRedisMaps(info, session);
        } catch (Exception e) {
            log.warn("从 Redis 读取用户缓存失败，回退到数据库查询: {}", e.getMessage());
            return null;
        }
    }

    private LoginVO fromRedisMaps(Map<Object, Object> info, Map<Object, Object> session) {
        LoginVO vo = new LoginVO();
        vo.setId(asString(info, "id"));
        vo.setUsername(asString(info, "username"));
        vo.setName(asString(info, "name"));
        vo.setEmail(asString(info, "email"));
        vo.setPhoneNumber(asString(info, "phone_number"));
        vo.setIsEnabled(asByte(info, "is_enabled"));
        vo.setLockedOut(asBoolean(info, "is_locked_out"));
        vo.setTenantId(asString(session, "tenant_id"));
        vo.setHospitalOrganizationId(asString(session, "hospital_organization_id"));
        vo.setOrganizationUnitId(asString(session, "organization_unit_id"));
        vo.setPractitionerId(asString(session, "practitioner_id"));
        vo.setPractitionerName(asString(session, "practitioner_name"));
        vo.setPractitionerStatus(asString(session, "practitioner_status"));
        return vo;
    }

    private void deleteUserCache() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                String userId = currentUser.getId();
                redisTemplate.delete(RedisConstant.USER_INFO_PREFIX + userId);
                redisTemplate.delete(RedisConstant.USER_SESSION_PREFIX + userId);
            }
        } catch (Exception e) {
            log.warn("退出登录时清除 Redis 缓存失败: {}", e.getMessage());
        }
    }

    // ---- 工具方法 ----

    private String asString(Map<Object, Object> map, String key) {
        Object value = map == null ? null : map.get(key);
        return value == null ? null : value.toString();
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
