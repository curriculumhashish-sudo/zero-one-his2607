package com.zeroone.star.sysmgr.service.user.impl;

import cn.hutool.crypto.digest.BCrypt;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.query.j1.user.UserListQuery;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.sysmgr.entity.user.User;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.exception.UserStatus;
import com.zeroone.star.sysmgr.mapper.user.UserMapper;
import com.zeroone.star.sysmgr.service.user.*;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;

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
    @Resource
    private IPractitionerService practitionerService;
    @Resource
    private IUserOrganizationUnitService userOrganizationUnitService;
    @Resource
    private IUserRoleService userRoleService;
    @Resource
    private IUserTenantService userTenantService;

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

    @Override
    public PageDTO<UserListVO> getList(UserListQuery query) {
        //1.构建查询条件
        QueryWrapper<User> wrapper = new QueryWrapper<>();
        if(StringUtils.hasText(query.getKeyword())){
            wrapper.and(
                    w->w.like("username",query.getKeyword())
                            .or().like("name",query.getKeyword())
                            .or().like("email",query.getKeyword()
                            ));
        }

        if(query.getIsActive()!=null){
            wrapper.eq("isActive",query.getIsActive());
        }

        //2.分页查询
        Page<User> page = new Page<>(query.getPageIndex(), query.getPageSize());
        baseMapper.selectPage(page, wrapper);

        //3.返回结果
        return PageDTO.create(page, user -> {
            UserListVO vo = new UserListVO();
            BeanUtils.copyProperties(user, vo);
            return vo;
        });
    }

    @Override
    public Boolean saveUser(UserAddDTO userAddDTO) {
        //1.效验用户名是否重复
        LambdaQueryWrapper<User> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(User::getUserName,userAddDTO.getUsername());
        if(baseMapper.selectCount(wrapper)>0){
            throw new SystemException(UserStatus.USERNAME_DUPLICATE);
        }
        //2.如果存在邮箱信息 效验邮箱是否重复
        if(StringUtils.hasText(userAddDTO.getEmail())){
            LambdaQueryWrapper<User> emailWrapper = new LambdaQueryWrapper<User>().eq(User::getEmail, userAddDTO.getEmail());
            if(baseMapper.selectCount(emailWrapper)>0){
                throw  new SystemException(UserStatus.EMAIL_DUPLICATE);
            }
        }

        //TODO：效验角色是否存在

        //3.转换类型
        User user = new User();
        BeanUtils.copyProperties(userAddDTO,user);
        user.setPasswordHash(BCrypt.hashpw(userAddDTO.getPassword()));

        //4.插入数据库
        baseMapper.insert(user);

        //5.获取用户id
        String userId = user.getId();

        //6.用户关联医务人员
        practitionerService.add(userId,userAddDTO);

        //7.用户关联科室
        userOrganizationUnitService.add(userId,userAddDTO.getDefaultOrgId());

        //8.用户关联角色
        userRoleService.add(userId,userAddDTO.getRoleIds());

        //9.用户关联租户
        userTenantService.add(userId);

        return true;
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
