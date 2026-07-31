package com.zeroone.star.project.components.user;

import cn.hutool.core.convert.Convert;
import cn.hutool.json.JSONObject;
import com.zeroone.star.project.components.jwt.JwtComponent;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import org.yaml.snakeyaml.util.UriEncoder;

import java.util.List;
import java.util.Map;

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
    private RedisTemplate<String, Object> redisTemplate;

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
        return UserDTO.builder()
                .id(Convert.toStr(userJsonObject.get("id")))
                .username(userJsonObject.getStr("sub"))
                .isEnabled(Convert.toByte(1))
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
     * 获取当前请求的租户ID
     * <p>数据来源：Redis Hash {@code his:login:context:{userId}} → field {@code tenantId}</p>
     * @return 租户ID
     * @throws RuntimeException 如果获取不到用户信息或租户ID
     */
    public String getCurrentTenantId() {
        try {
            // 1. 获取当前用户ID
            UserDTO currentUser = getCurrentUser();
            String userId = currentUser.getId();

            // 2. 构建 Redis Key
            String redisKey = "his:login:context:" + userId;

            // 3. 从 Hash 中取 tenantId
            Object tenantObj = redisTemplate.opsForHash().get(redisKey, "tenantId");
            if (tenantObj == null) {
                throw new RuntimeException("未找到用户 [" + userId + "] 的租户信息");
            }
            return tenantObj.toString();

        } catch (RuntimeException e) {
            throw e;
        } catch (Exception e) {
            throw new RuntimeException("获取当前租户ID失败", e);
        }
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
}
