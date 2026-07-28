package com.zeroone.star.sysmgr.constant;

public interface DeptRedisConstant {
    /**
     * 用户可访问科室权限缓存有效期：6小时 单位秒
     */
    long USER_ALLOW_DEPT_CACHE_TTL = 21600L;

    /**
     * 用户登录选中科室会话有效期，跟随登录会话
     */
    long USER_SELECTED_DEPT_TTL = 36000L;

    /**
     * 缓存：用户有权限的科室列表
     * key: his:login:allow_dept:{tenantId}:{userId}
     */
    String USER_ALLOW_DEPT_KEY_PREFIX = "his:login:allow_dept:";
    static String getUserAllowDeptKey(String tenantId, String userId) {
        return USER_ALLOW_DEPT_KEY_PREFIX + tenantId + ":" + userId;
    }

    /**
     * 会话存储：用户登录选中的科室ID集合
     * key: his:login:selected_dept:{userId}
     */
    String USER_SELECTED_DEPT_KEY_PREFIX = "his:login:selected_dept:";
    static String getUserSelectedDeptKey(String userId) {
        return USER_SELECTED_DEPT_KEY_PREFIX + userId;
    }
}
