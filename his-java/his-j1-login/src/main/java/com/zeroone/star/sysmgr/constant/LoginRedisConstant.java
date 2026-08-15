package com.zeroone.star.sysmgr.constant;

import static com.zeroone.star.project.constant.RedisConstant.USER_SESSION_PREFIX;

public final class LoginRedisConstant {

    private LoginRedisConstant() {}

    public static final long TENANT_LIST_TTL = 86400;
    public static final long HOSPITAL_LIST_TTL = 86400;
    public static final long USER_SESSION_TTL = 36000;

    public static String loginContextKey(String userId) {
        return "his:login:context:" + userId;
    }

    public static String loginTenantsKey(String userId) {
        return "his:login:tenants:" + userId;
    }

    public static String loginHospitalsKey(String tenantId) {
        return "his:login:hospitals:" + tenantId;
    }

    public static String userSessionKey(String userId) {
        return USER_SESSION_PREFIX + userId;
    }
}