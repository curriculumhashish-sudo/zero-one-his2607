package com.zeroone.star.sysmgr.service.login;

import com.zeroone.star.sysmgr.entity.login.LoginUserProfileDO;

import java.util.List;
import java.util.Map;

/**
 * 登录接口使用的 Redis 上下文边界。所有登录相关 Key、Hash 字段和 TTL
 * 均集中在此服务，调用方不得直接操作 Redis。
 */
public interface LoginContextService {

    void saveContext(String userId, String token, LoginUserProfileDO profile,
                     List<String> roles, String permissionsJson);

    Map<String, String> loadContext(String userId);

    void refreshContext(String userId);

    void removeContext(String userId);

    void saveUserInfo(String userId, LoginUserProfileDO profile);

    Map<String, String> loadUserInfo(String userId);

    void removeUserInfo(String userId);
}
