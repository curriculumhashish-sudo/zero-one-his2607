package com.zeroone.star.project.components.user;

import cn.hutool.core.convert.Convert;
import cn.hutool.json.JSONObject;
import com.zeroone.star.project.components.jwt.JwtComponent;
import com.zeroone.star.project.constant.RedisConstant;
import jakarta.annotation.Resource;
import jakarta.servlet.http.HttpServletRequest;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import org.yaml.snakeyaml.util.UriEncoder;

/**
 * <p>
 * 鎻忚堪锛氳幏鍙栫櫥褰曠敤鎴蜂俊鎭?
 * </p>
 * <p>鐗堟潈锛?copy;01鏄熺悆</p>
 * <p>鍦板潃锛?1鏄熺悆鎬婚儴</p>
 * @author 闃夸紵瀛﹂暱
 * @version 1.0.0
 */
@Component
public class UserHolder {
    @Resource
    JwtComponent jwtComponent;

    @Resource
    ApplicationContext applicationContext;

    /**
     * 浠庤姹傚ご涓幏鍙栫敤鎴蜂俊鎭?
     * @return 鐢ㄦ埛淇℃伅
     * @throws Exception 瑙ｆ瀽澶辫触鎶涘嚭寮傚父
     */
    @SuppressWarnings("MismatchedQueryAndUpdateOfCollection")
    public UserDTO getCurrentUser() throws Exception {
        // 浠嶩eader涓幏鍙栫敤鎴蜂俊鎭?
        ServletRequestAttributes servletRequestAttributes = (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
        HttpServletRequest request = servletRequestAttributes.getRequest();
        String userStr = request.getHeader("user");
        JSONObject userJsonObject;
        // 涓嶆槸閫氳繃缃戝叧杩囨潵鐨勶紝閭ｄ箞鎵ц瑙ｆ瀽楠岃瘉JWT
        if (userStr == null) {
            //浠巘oken涓В鏋愮敤鎴蜂俊鎭苟璁剧疆鍒癏eader涓幓
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

//        // HARD_CODE 鍦ㄦ病鏈夊姙娉曚娇鐢╰oken鏃跺€欏彲浠ヤ慨鏀硅繖閲岀殑浠ｇ爜浼€犵敤鎴蜂俊鎭紝娉ㄦ剰浼€犵敤鎴蜂唬鐮佷笉瑕佹彁浜ゅ埌浠撳簱涓?
//        userJsonObject = new JSONObject();
//        userJsonObject.putOnce("id", 1);
//        userJsonObject.putOnce("user_name", "鐜嬮夯瀛?);
//        ArrayList<Object> roles = new ArrayList<>();
//        roles.add("ROLE_ADMIN");
//        userJsonObject.putOnce("authorities", roles);

        // FIXME: 濡傛灉瑕佹墿灞曠敤鎴蜂俊鎭紝闇€瑕佷慨鏀硅繖閲岀殑浠ｇ爜
        return UserDTO.builder()
                .id(Convert.toStr(userJsonObject.get("id")))
                .username(userJsonObject.getStr("sub"))
                .isEnabled(Convert.toByte(1))
                .roles(Convert.toList(String.class, userJsonObject.get("roles")))
                .build();
    }

    /**
     * 浠庤姹傚ご涓幏鍙栧綋鍓嶈姹傜殑token
     * @return 娌℃湁鑾峰彇鍒拌繑鍥瀗ull
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
     * 鑾峰彇褰撳墠鐢ㄦ埛閫夋嫨鐨勭鎴稩D
     * @return 褰撳墠绉熸埛ID
     */
    public String getCurrentTenantId() {
        try {
            UserDTO currentUser = getCurrentUser();
            if (currentUser == null || !StringUtils.hasText(currentUser.getId())) {
                throw new IllegalStateException("鏃犳硶鑾峰彇褰撳墠鐢ㄦ埛ID");
            }
            if (applicationContext == null) {
                throw new IllegalStateException("RedisTemplate鏈垵濮嬪寲锛屾棤娉曡幏鍙栧綋鍓嶇鎴稩D");
            }

            String userId = currentUser.getId();
            Object tenantId = getRedisHashValue(
                    RedisConstant.LOGIN_CONTEXT_PREFIX + userId,
                    RedisConstant.LOGIN_CONTEXT_TENANT_ID_FIELD
            );
            if (tenantId == null) {
                tenantId = getRedisHashValue(
                        RedisConstant.USER_SESSION_PREFIX + userId,
                        RedisConstant.USER_SESSION_TENANT_ID_FIELD
                );
            }
            if (tenantId == null || !StringUtils.hasText(tenantId.toString())) {
                throw new IllegalStateException("鏈壘鍒板綋鍓嶇敤鎴风殑绉熸埛ID");
            }
            return tenantId.toString();
        } catch (RuntimeException e) {
            throw e;
        } catch (Exception e) {
            throw new IllegalStateException("鑾峰彇褰撳墠绉熸埛ID澶辫触", e);
        }
    }

    private Object getRedisHashValue(String key, String field) {
        try {
            Object redisTemplate = applicationContext.getBean("redisTemplate");
            Class<?> redisOperationsClass = Class.forName("org.springframework.data.redis.core.RedisOperations");
            Class<?> hashOperationsClass = Class.forName("org.springframework.data.redis.core.HashOperations");
            Object hashOperations = redisOperationsClass.getMethod("opsForHash").invoke(redisTemplate);
            return hashOperationsClass.getMethod("get", Object.class, Object.class).invoke(hashOperations, key, field);
        } catch (BeansException e) {
            throw new IllegalStateException("RedisTemplate is not initialized, cannot get current tenant id", e);
        } catch (ReflectiveOperationException e) {
            throw new IllegalStateException("Failed to get current tenant id", e);
        }
    }
}
