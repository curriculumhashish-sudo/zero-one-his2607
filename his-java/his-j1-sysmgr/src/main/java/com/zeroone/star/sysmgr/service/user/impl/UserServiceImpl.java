package com.zeroone.star.sysmgr.service.user.impl;

import com.zeroone.star.sysmgr.entity.user.User;
import com.zeroone.star.sysmgr.mapper.user.UserMapper;
import com.zeroone.star.sysmgr.service.user.IUserService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import org.springframework.stereotype.Service;

/**
 * <p>
 * 用户主表 服务实现类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
@Service
public class UserServiceImpl extends ServiceImpl<UserMapper, User> implements IUserService {

}
