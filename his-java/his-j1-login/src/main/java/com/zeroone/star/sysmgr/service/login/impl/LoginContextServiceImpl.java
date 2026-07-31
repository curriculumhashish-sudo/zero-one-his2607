package com.zeroone.star.sysmgr.service.login.impl;

import com.zeroone.star.sysmgr.entity.login.LoginUserProfileDO;
import com.zeroone.star.sysmgr.service.login.LoginContextService;
import jakarta.annotation.Resource;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.stereotype.Service;

import java.time.Instant;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@Service
public class LoginContextServiceImpl implements LoginContextService {

    private static final String LOGIN_CONTEXT_PREFIX = "his:login:context:";
    private static final String USER_INFO_PREFIX = "user:info:";
    private static final long LOGIN_CONTEXT_TTL_SECONDS = 7200L;
    private static final long USER_INFO_TTL_SECONDS = 3600L;
    private static final List<String> REQUIRED_CONTEXT_FIELDS = List.of(
            "token", "userId", "username", "realName", "tenantId", "tenantName",
            "hospitalId", "hospitalName", "deptId", "deptName", "role", "permissions",
            "loginTime", "lastActiveTime", "email", "phone_number", "is_enabled",
            "is_locked_out", "practitionerId", "practitionerName", "practitionerStatus"
    );

    @Resource
    private StringRedisTemplate stringRedisTemplate;

    @Override
    public void saveContext(String userId, String token, LoginUserProfileDO profile,
                            List<String> roles, String permissionsJson) {
        Map<String, String> context = new LinkedHashMap<>();
        String now = Instant.now().toString();
        // 设计规定的固定字段；空值一律使用空串，禁止产生不完整 Hash。
        context.put("token", value(token));
        context.put("userId", value(userId));
        context.put("username", value(profile == null ? null : profile.getUsername()));
        context.put("realName", value(profile == null ? null : profile.getName()));
        context.put("tenantId", value(profile == null ? null : profile.getTenantId()));
        context.put("tenantName", value(profile == null ? null : profile.getTenantName()));
        context.put("hospitalId", value(profile == null ? null : profile.getHospitalOrganizationId()));
        context.put("hospitalName", value(profile == null ? null : profile.getHospitalName()));
        context.put("deptId", value(profile == null ? null : profile.getOrganizationUnitId()));
        context.put("deptName", value(profile == null ? null : profile.getOrganizationUnitName()));
        context.put("role", roles == null ? "" : roles.stream().collect(Collectors.joining(",")));
        context.put("permissions", value(permissionsJson));
        context.put("loginTime", now);
        context.put("lastActiveTime", now);
        // 补充 LoginVO 的既有字段，确保 context 命中与数据库路径返回一致。
        context.put("email", value(profile == null ? null : profile.getEmail()));
        context.put("phone_number", value(profile == null ? null : profile.getPhoneNumber()));
        context.put("is_enabled", value(profile == null ? null : profile.getIsEnabled()));
        context.put("is_locked_out", value(profile == null ? null : profile.getLockedOut()));
        context.put("practitionerId", value(profile == null ? null : profile.getPractitionerId()));
        context.put("practitionerName", value(profile == null ? null : profile.getPractitionerName()));
        context.put("practitionerStatus", value(profile == null ? null : profile.getPractitionerStatus()));
        String key = contextKey(userId);
        stringRedisTemplate.opsForHash().putAll(key, context);
        stringRedisTemplate.expire(key, LOGIN_CONTEXT_TTL_SECONDS, java.util.concurrent.TimeUnit.SECONDS);
    }

    @Override
    public Map<String, String> loadContext(String userId) {
        Map<String, String> context = loadHash(contextKey(userId));
        // 兼容历史遗留的不完整 Hash：不向调用方暴露半成品，触发 MySQL 回源并重建。
        return context.keySet().containsAll(REQUIRED_CONTEXT_FIELDS) ? context : Map.of();
    }

    @Override
    public void refreshContext(String userId) {
        String key = contextKey(userId);
        if (Boolean.TRUE.equals(stringRedisTemplate.hasKey(key))) {
            stringRedisTemplate.opsForHash().put(key, "lastActiveTime", Instant.now().toString());
        }
    }

    @Override
    public void removeContext(String userId) {
        stringRedisTemplate.delete(contextKey(userId));
    }

    @Override
    public void saveUserInfo(String userId, LoginUserProfileDO profile) {
        Map<String, String> userInfo = new LinkedHashMap<>();
        // 设计规定的固定字段；profile 缺失时仍完整写入空字段。
        userInfo.put("id", value(userId));
        userInfo.put("username", value(profile == null ? null : profile.getUsername()));
        userInfo.put("name", value(profile == null ? null : profile.getName()));
        userInfo.put("email", value(profile == null ? null : profile.getEmail()));
        userInfo.put("phone_number", value(profile == null ? null : profile.getPhoneNumber()));
        userInfo.put("is_enabled", value(profile == null ? null : profile.getIsEnabled()));
        userInfo.put("is_locked_out", value(profile == null ? null : profile.getLockedOut()));
        String key = userInfoKey(userId);
        stringRedisTemplate.opsForHash().putAll(key, userInfo);
        stringRedisTemplate.expire(key, USER_INFO_TTL_SECONDS, java.util.concurrent.TimeUnit.SECONDS);
    }

    @Override
    public Map<String, String> loadUserInfo(String userId) {
        return loadHash(userInfoKey(userId));
    }

    @Override
    public void removeUserInfo(String userId) {
        stringRedisTemplate.delete(userInfoKey(userId));
    }

    private Map<String, String> loadHash(String key) {
        Map<Object, Object> entries = stringRedisTemplate.opsForHash().entries(key);
        if (entries.isEmpty()) {
            return Map.of();
        }
        Map<String, String> result = new LinkedHashMap<>();
        entries.forEach((field, fieldValue) -> result.put(String.valueOf(field), value(fieldValue)));
        return result;
    }

    private String contextKey(String userId) {
        return LOGIN_CONTEXT_PREFIX + userId;
    }

    private String userInfoKey(String userId) {
        return USER_INFO_PREFIX + userId;
    }

    private static String value(Object value) {
        return value == null ? "" : String.valueOf(value);
    }
}
