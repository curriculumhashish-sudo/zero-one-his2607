package com.zeroone.star.sysmgr.service.user.impl;

import cn.hutool.crypto.digest.BCrypt;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.sysmgr.entity.user.User;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.exception.UserStatus;
import com.zeroone.star.sysmgr.mapper.user.UserMapper;
import com.zeroone.star.sysmgr.service.user.IUserService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
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

    @Resource
    private UserHolder userHolder;

    @Override
    public boolean softDeleteById(String id){
        //1.查询用户
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        //2.获取删除用户用户id
        String deletedBy=getCurrentUserId();
        //3.删除用户
        return baseMapper.softDeleteById(id, deletedBy) > 0;
    }

    @Override
    public boolean updateStatus(String id, Integer isActive) {
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        return baseMapper.updateStatus(id, isActive) > 0;
    }

    @Override
    public boolean lockUser(String id, Integer isLockedOut) {
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        return baseMapper.lockUser(id, isLockedOut) > 0;
    }

    @Override
    public boolean updatePassword(String id, String newPassword) {
        User user = baseMapper.selectById(id);
        if(user==null){
            throw new SystemException(UserStatus.USER_NOT_FIND);
        }
        String hashed = BCrypt.hashpw(newPassword);
        return baseMapper.updatePassword(id, hashed) > 0;
    }

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user.getId();
        } catch (Exception e) {
            throw new SystemException(UserStatus.GET_USER_FAILED);
        }
    }
}
