package com.zeroone.star.sysmgr.service.user;

import com.zeroone.star.sysmgr.entity.user.UserTenant;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 用户租户关联 服务类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-27
 */
public interface IUserTenantService extends IService<UserTenant> {

    /**
     * 添加用户与租户关联信息
     * @param userId 用户id
     */
    void add(String userId);
}
