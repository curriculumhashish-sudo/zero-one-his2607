package com.zeroone.star.project.constant;

/**
 * <p>
 * 描述：Redis相关常量
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface RedisConstant {
    /**
     * 角色资源映射Map的key
     */
    String RESOURCE_ROLES_MAP = "auth:res:roles";
    /**
     * 访问凭证的key前缀
     */
    String ACCESS_TOKEN_PREFIX = "auth:at:";
    /**
     * 刷新凭证的key前缀
     */
    String REFRESH_TOKEN_PREFIX = "auth:rt:";
    /**
     * 用户基本信息缓存的key前缀（Hash）
     */
    String USER_INFO_PREFIX = "user:info:";
    /**
     * 用户会话上下文缓存的key前缀（Hash）
     */
    String USER_SESSION_PREFIX = "user:session:";
}
