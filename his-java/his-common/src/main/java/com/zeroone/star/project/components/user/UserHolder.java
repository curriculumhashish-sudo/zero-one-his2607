package com.zeroone.star.project.components.user;

import cn.hutool.core.convert.Convert;
import cn.hutool.json.JSONObject;
import com.zeroone.star.project.components.jwt.JwtComponent;
import com.zeroone.star.project.constant.RedisConstant;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.data.redis.core.StringRedisTemplate;
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
    @Resource
    JwtComponent jwtComponent;

    @Resource
    StringRedisTemplate stringRedisTemplate;

    /**
     * 从请求头中获取用户信息
     * @return 用户信息
     * @throws Exception 解析失败抛出异常
     */
    @SuppressWarnings({"MismatchedQueryAndUpdateOfCollection", "unchecked"})
    public UserDTO getCurrentUser() throws Exception {
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
//        userJsonObject.putOnce("sub", "王麻子");
//        ArrayList<Object> roles = new ArrayList<>();
//        roles.add("ROLE_ADMIN");
//        userJsonObject.putOnce("roles", roles);

        // FIXME: 如果要扩展用户信息，需要修改这里的代码
        String username = userJsonObject.getStr("user_name");
        if (!StringUtils.hasText(username)) {
            username = userJsonObject.getStr("sub");
        }
        Byte isEnabled = Convert.toByte(userJsonObject.get("is_active"));
        return UserDTO.builder()
                .id(Convert.toStr(userJsonObject.get("id")))
                .username(username)
                .isEnabled(isEnabled == null ? (byte) 1 : isEnabled)
                .roles(Convert.toList(String.class, userJsonObject.get("roles")))
                .name(userJsonObject.getStr("name"))
                .organizationUnits(Convert.toMap(String.class, String.class, userJsonObject.get("organizationUnits")))
                .practitionerId(userJsonObject.getStr("practitionerId"))
                .practitionerName(userJsonObject.getStr("practitionerName"))
                .titleName(userJsonObject.getStr("titleName"))
                .titleCode(userJsonObject.getStr("titleCode"))
                .build();
    }

    /**
     * 从请求头中获取当前请求的token
     * @return 没有获取到返回null
     */
    public String getCurrentToken() {
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
            String userId = currentUser.getId();
            Object tenantId = stringRedisTemplate.opsForHash().get(RedisConstant.USER_SESSION_PREFIX + userId, RedisConstant.USER_SESSION_TENANT_ID_FIELD);
            if (tenantId == null || !StringUtils.hasText(tenantId.toString())) {
                throw new IllegalStateException("未找到当前用户的租户ID");
            }
            return tenantId.toString();
        } catch (Exception e) {
            throw new IllegalStateException("获取当前租户ID失败", e);
        }
    }

    /**
     * 获取当前用户选择的部门ID
     * @return 当前部门ID
     */
    public String getCurrentDeptId() {
        try {
            UserDTO currentUser = getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                throw new IllegalStateException("Cannot get current user id");
            }
            String userId = currentUser.getId();
            Object deptId = stringRedisTemplate.opsForHash().get(
                    RedisConstant.LOGIN_CONTEXT_PREFIX + userId,
                    RedisConstant.LOGIN_CONTEXT_DEPT_ID_FIELD
            );
            if (deptId == null || !StringUtils.hasText(deptId.toString())) {
                throw new IllegalStateException("Current user has no department/organization id");
            }
            return deptId.toString();
        } catch (Exception e) {
            throw new IllegalStateException("Failed to get current department/organization id", e);
        }
    }
}
