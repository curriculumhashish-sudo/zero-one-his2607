package com.zeroone.star.sysmgr.service.user.impl;

import cn.hutool.crypto.digest.BCrypt;
import com.zeroone.star.sysmgr.entity.user.User;
import com.zeroone.star.sysmgr.mapper.user.UserMapper;
import com.zeroone.star.sysmgr.service.user.IUserService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 系统用户 服务实现类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Service
public class UserServiceImpl extends ServiceImpl<UserMapper, User> implements IUserService {

    @Override
    public boolean softDeleteById(String id, String deletedBy) {
        return baseMapper.softDeleteById(id, deletedBy) > 0;
    }

    @Override
    public boolean updateStatus(String id, Integer isActive) {
        return baseMapper.updateStatus(id, isActive) > 0;
    }

    @Override
    public boolean lockUser(String id, Integer isLockedOut) {
        return baseMapper.lockUser(id, isLockedOut) > 0;
    }

    @Override
    public boolean updatePassword(String id, String newPassword) {
        String hashed = BCrypt.hashpw(newPassword);
        return baseMapper.updatePassword(id, hashed) > 0;
    }
}
