package com.zeroone.star.sysmgr.service.user.impl;

import cn.hutool.crypto.digest.BCrypt;
import cn.hutool.json.JSONObject;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j1.user.UserAddDTO;
import com.zeroone.star.project.query.j1.user.TenantQuery;
import com.zeroone.star.project.vo.j1.user.UserListVO;
import com.zeroone.star.sysmgr.entity.user.User;
import com.zeroone.star.sysmgr.exception.SystemException;
import com.zeroone.star.sysmgr.exception.UserStatus;
import com.zeroone.star.sysmgr.mapper.user.UserMapper;
import com.zeroone.star.sysmgr.service.user.IUserService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.beans.BeanUtils;
import org.springframework.stereotype.Service;
import org.springframework.util.StringUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;

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

    @Override
    public PageDTO<UserListVO> getList(TenantQuery tenantQuery) {
        QueryWrapper<User> wrapper = new QueryWrapper<>();
        wrapper.eq("is_deleted",false);
        if(StringUtils.hasText(tenantQuery.getKeyword())){
            wrapper.and(w->w.like("user_name",tenantQuery.getKeyword())
                    .or().like("email",tenantQuery.getKeyword())
                    .or().like("name",tenantQuery.getKeyword())
                    );
        }

        if(tenantQuery.getIsActive()!=null){
            wrapper.eq("is_active", tenantQuery.getIsActive());
        }

        wrapper.orderByAsc("user_name");
        Page<User> page = new Page<>(tenantQuery.getPageIndex(), tenantQuery.getPageSize());
        baseMapper.selectPage(page, wrapper);
        return PageDTO.create(page, UserListVO::new);
    }

    @Override
    public Boolean saveUser(UserAddDTO userAddDTO) {
        //1.效验用户名称是否唯一
        LambdaQueryWrapper<User> wrapper = new LambdaQueryWrapper<>();
        wrapper.eq(User::getUserName, userAddDTO.getUsername())
                .eq(User::getIsDeleted,0);
        if (this.baseMapper.selectCount(wrapper) > 0){
            throw new SystemException(UserStatus.USER_NAME_EXIST);
        }
        //2.新增用户
        User user = new User();
        BeanUtils.copyProperties(userAddDTO,user);
        user.setPasswordHash(BCrypt.hashpw(userAddDTO.getPassword()));

        //3.获取用户id
        //4.关联用户角色
        //5.关联用户科室
        //6.关联用户租户
        //6.关联医务人员
        return null;
    }

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user.getId();
        } catch (Exception e) {
            throw new SystemException(UserStatus.GET_USER_FAILED);
        }
    }

    private String getTenantId() {
        ServletRequestAttributes attrs =
                (ServletRequestAttributes) RequestContextHolder.getRequestAttributes();
        if (attrs == null) {
            return "1";
        }
        String userStr = attrs.getRequest().getHeader("user");
        if (!StringUtils.hasText(userStr)) {
            return "1";
        }
        JSONObject userJson = new JSONObject(userStr);
        String tenantId = userJson.getStr("tenant_id");
        if (!StringUtils.hasText(tenantId)) {
            return "1";
        }
        return tenantId;
    }
}
