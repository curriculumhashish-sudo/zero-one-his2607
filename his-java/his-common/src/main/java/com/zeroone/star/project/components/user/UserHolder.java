package com.zeroone.star.project.components.user;

import cn.hutool.core.convert.Convert;
import cn.hutool.json.JSONObject;
import com.zeroone.star.project.components.jwt.JwtComponent;
import com.zeroone.star.project.constant.RedisConstant;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import org.yaml.snakeyaml.util.UriEncoder;

/**
 * <p>
 * 描述：获取登录用户信息
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Component
public class UserHolder {
    private static final String LOCAL_TEST_USER_ID = "1";
    private static final String LOCAL_TEST_TENANT_ID = "1";
    private static final String LOCAL_TEST_DEPT_ID = "1";

    @Value("${spring.application.name:}")
    private String applicationName;

    @Value("${j3.auth-bypass.enabled:false}")
    private boolean j3AuthBypassEnabled;

    @Resource
    JwtComponent jwtComponent;
    @Resource
    ApplicationContext applicationContext;

    /**
     * 从请求头中获取用户信息
     * @return 用户信息
     * @throws Exception 解析失败抛出异常
     */
    @SuppressWarnings("MismatchedQueryAndUpdateOfCollection")
    public UserDTO getCurrentUser() throws Exception {
        if (isJ3AuthBypassEnabled()) {
            JSONObject userJsonObject = new JSONObject();
            fillLocalTestUser(userJsonObject);
            return buildUserDto(userJsonObject);
        }

        // 从Header中获取用户信息
        ServletRequestAttributes servletRequestAttributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
        HttpServletRequest request = servletRequestAttributes.getRequest();
        String userStr = request.getHeader("user");
        JSONObject userJsonObject;
        // 不是通过网关过来的，那么执行解析验证JWT
        if (userStr == null) {
            //从token中解析用户信息并设置到Header中去
            String realToken = request.getHeader("Authorization");
            if (StringUtils.hasText(realToken)) {
                realToken = realToken.replace("Bearer ", "");
                userStr = jwtComponent.defaultRsaVerify(realToken);
                userJsonObject = new JSONObject(userStr);
            } else {
                userJsonObject = new JSONObject();
            }
        } else {
            userStr = UriEncoder.decode(userStr);
            userJsonObject = new JSONObject(userStr);
        }

//        // HARD_CODE 在没有办法使用token时候可以修改这里的代码伪造用户信息，注意伪造用户代码不要提交到仓库中
//        userJsonObject = new JSONObject();
//        userJsonObject.putOnce("id", 1);
//        userJsonObject.putOnce("user_name", "王麻子");
//        ArrayList<Object> roles = new ArrayList<>();
//        roles.add("ROLE_ADMIN");
//        userJsonObject.putOnce("authorities", roles);

        // FIXME: 如果要扩展用户信息，需要修改这里的代码
        return buildUserDto(userJsonObject);
    }

    private UserDTO buildUserDto(JSONObject userJsonObject) {
        return UserDTO.builder()
                .id(Convert.toStr(userJsonObject.get("id")))
                .username(userJsonObject.getStr("sub"))
                .isEnabled(Convert.toByte(1))
                .roles(Convert.toList(String.class, userJsonObject.get("roles")))
                .build();
    }

    /**
     * 从请求头中获取当前请求的token
     * @return 没有获取到返回null
     */
    public String getCurrentToken() {
        if (isJ3AuthBypassEnabled()) {
            return null;
        }

        ServletRequestAttributes servletRequestAttributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
        HttpServletRequest request = servletRequestAttributes.getRequest();
        String token = request.getHeader("Authorization");
        if (!StringUtils.hasText(token)) {
            return null;
        }
        return token.replace("Bearer ", "");
    }

    /**
     * 获取当前用户选择的租户ID
     * @return 当前租户ID
     */
    public String getCurrentTenantId() {
        try {
            UserDTO currentUser = getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                throw new IllegalStateException("无法获取当前用户ID");
            }
            if (applicationContext == null) {
                throw new IllegalStateException("RedisTemplate未初始化，无法获取当前租户ID");
            }

            String userId = currentUser.getId();
            Object tenantId = getRedisHashValue(
                    RedisConstant.LOGIN_CONTEXT_PREFIX + userId,
                    RedisConstant.LOGIN_CONTEXT_TENANT_ID_FIELD
            );
            if (tenantId == null || !StringUtils.hasText(tenantId.toString())) {
                tenantId = getRedisHashValue(
                        RedisConstant.USER_SESSION_PREFIX + userId,
                        RedisConstant.USER_SESSION_TENANT_ID_FIELD
                );
            }
            if (tenantId == null || !StringUtils.hasText(tenantId.toString())) {
                if (isJ3AuthBypassEnabled()) {
                    return LOCAL_TEST_TENANT_ID;
                }
                throw new IllegalStateException("未找到当前用户的租户ID");
            }
            return tenantId.toString();
        } catch (RuntimeException e) {
            if (isJ3AuthBypassEnabled()) {
                return LOCAL_TEST_TENANT_ID;
            }
            throw e;
        } catch (Exception e) {
            if (isJ3AuthBypassEnabled()) {
                return LOCAL_TEST_TENANT_ID;
            }
            throw new IllegalStateException("获取当前租户ID失败", e);
        }
    }

    /**
     * Get current user's department/organization id from login context.
     *
     * @return current department/organization id
     */
    public String getCurrentDeptId() {
        try {
            UserDTO currentUser = getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                throw new IllegalStateException("Cannot get current user id");
            }
            if (applicationContext == null) {
                throw new IllegalStateException("RedisTemplate is not initialized");
            }

            Object deptId = getRedisHashValue(
                    RedisConstant.LOGIN_CONTEXT_PREFIX + currentUser.getId(),
                    RedisConstant.LOGIN_CONTEXT_DEPT_ID_FIELD
            );
            if (deptId == null || !StringUtils.hasText(deptId.toString())) {
                if (isJ3AuthBypassEnabled()) {
                    return LOCAL_TEST_DEPT_ID;
                }
                throw new IllegalStateException("Current user has no department/organization id");
            }
            return deptId.toString();
        } catch (RuntimeException e) {
            if (isJ3AuthBypassEnabled()) {
                return LOCAL_TEST_DEPT_ID;
            }
            throw e;
        } catch (Exception e) {
            if (isJ3AuthBypassEnabled()) {
                return LOCAL_TEST_DEPT_ID;
            }
            throw new IllegalStateException("Failed to get current department/organization id", e);
        }
    }

    private boolean isJ3AuthBypassEnabled() {
        return j3AuthBypassEnabled && applicationName != null && applicationName.startsWith("his-j3-");
    }

    private void fillLocalTestUser(JSONObject userJsonObject) {
        userJsonObject.putOnce("id", LOCAL_TEST_USER_ID);
        userJsonObject.putOnce("sub", "j3-test");
        userJsonObject.putOnce("roles", java.util.List.of("ROLE_ADMIN"));
    }

    private Object getRedisHashValue(String key, String field) {
        try {
            Object redisTemplate = applicationContext.getBean("redisTemplate");
            Class<?> redisOperationsClass = Class.forName("org.springframework.data.redis.core.RedisOperations");
            Class<?> hashOperationsClass = Class.forName("org.springframework.data.redis.core.HashOperations");
            Object hashOperations = redisOperationsClass.getMethod("opsForHash").invoke(redisTemplate);
            return hashOperationsClass.getMethod("get", Object.class, Object.class).invoke(hashOperations, key, field);
        } catch (BeansException e) {
            throw new IllegalStateException("RedisTemplate未初始化，无法获取当前租户ID", e);
        } catch (ReflectiveOperationException e) {
            throw new IllegalStateException("获取当前租户ID失败", e);
        }
    }
}
