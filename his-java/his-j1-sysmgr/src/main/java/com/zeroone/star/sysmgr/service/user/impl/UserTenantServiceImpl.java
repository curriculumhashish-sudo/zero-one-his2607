package com.zeroone.star.sysmgr.service.user.impl;

import com.zeroone.star.sysmgr.entity.user.UserTenant;
import com.zeroone.star.sysmgr.mapper.user.UserTenantMapper;
import com.zeroone.star.sysmgr.service.user.IUserTenantService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 用户租户关联 服务实现类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-27
 */
@Service
public class UserTenantServiceImpl extends ServiceImpl<UserTenantMapper, UserTenant> implements IUserTenantService {

    @Override
    public void add(String userId) {

    }
}
