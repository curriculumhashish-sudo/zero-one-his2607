package com.zeroone.star.project.constant;

/**
 * <p>
 * 描述：Redis相关常量
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
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
     * 用户登录上下文key前缀
     */
    String LOGIN_CONTEXT_PREFIX = "his:login:context:";
    /**
     * 用户登录上下文中的租户ID字段
     */
    String LOGIN_CONTEXT_TENANT_ID_FIELD = "tenantId";
    /**
     * Department/organization id in the login context hash.
     */
    String LOGIN_CONTEXT_DEPT_ID_FIELD = "deptId";
    /**
     * 用户会话key前缀
     */
    String USER_SESSION_PREFIX = "user:session:";
    /**
     * 用户会话中的租户ID字段
     */
    String USER_SESSION_TENANT_ID_FIELD = "tenant_id";

    /**
     * 用户登录上下文（Hash），包含 tenantId
     */
    String LOGIN_CONTEXT = "his:login:context:";

    /**
     * 用户会话上下文（Hash），包含 tenant_id
     */
    String USER_SESSION = "user:session:";
}
