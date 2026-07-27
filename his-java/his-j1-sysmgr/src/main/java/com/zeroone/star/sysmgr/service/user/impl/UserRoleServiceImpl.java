package com.zeroone.star.sysmgr.service.user.impl;

import com.zeroone.star.sysmgr.entity.user.UserRole;
import com.zeroone.star.sysmgr.mapper.user.UserRoleMapper;
import com.zeroone.star.sysmgr.service.user.IUserRoleService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * <p>
 * 用户角色关联 服务实现类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Service
public class UserRoleServiceImpl extends ServiceImpl<UserRoleMapper, UserRole> implements IUserRoleService {

    @Override
    public void add(String userId, List<String> roleIds) {

    }
}
