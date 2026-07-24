package com.zeroone.stat.sysmgr.service.impl;

import cn.hutool.crypto.digest.BCrypt;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.TenantUserDTO;
import com.zeroone.star.project.query.j2_sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.stat.sysmgr.entity.User;
import com.zeroone.stat.sysmgr.mapper.UserMapper;
import com.zeroone.stat.sysmgr.mapper.UserTenantMapper;
import com.zeroone.stat.sysmgr.service.ITenantUserService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;

@Service
public class TenantUserServiceImpl implements ITenantUserService {

    @Resource
    private UserMapper userMapper;

    @Resource
    private UserTenantMapper userTenantMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    public JsonVO<PageDTO<TenantUserDTO>> listTenantUsers(TenantUserQuery query) {
        Page<TenantUserDTO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        IPage<TenantUserDTO> result = userTenantMapper.listTenantUsers(
                page,
                query.getNewTenant(),
                query.getUserName(),
                query.getName()
        );
        return JsonVO.success(PageDTO.create((Page<TenantUserDTO>) result));
    }

    @Override
    public JsonVO<String> addTenantUsers(AddTenantUserDTO data) {
        if (data.getUsers() == null || data.getUsers().isEmpty()) {
            return JsonVO.fail("用户列表不能为空");
        }
        String currentUser = getCurrentUsername();
        for (AddTenantUserDTO.TenantUserItem item : data.getUsers()) {
            User user = userMapper.selectOne(
                    new LambdaQueryWrapper<User>()
                            .eq(User::getUserName, item.getUsername())
                            .eq(User::getIsDeleted, 0)
            );
            if (user == null) {
                user = new User();
                user.setUserName(item.getUsername());
                user.setName(item.getName());
                user.setPasswordHash(BCrypt.hashpw("123456"));
                user.setCreatedAt(LocalDateTime.now());
                user.setCreatedBy(currentUser);
                userMapper.insert(user);
            }
            userTenantMapper.addOrReactivateUserTenant(
                    String.valueOf(IdWorker.getId()),
                    user.getId(),
                    data.getTenantId(),
                    currentUser,
                    currentUser
            );
        }
        return JsonVO.success("添加成功");
    }

    @Override
    public JsonVO<String> removeTenantUsers(RemoveTenantUserDTO data) {
        if (data.getUsernames() == null || data.getUsernames().isEmpty()) {
            return JsonVO.fail("用户名列表不能为空");
        }
        String currentUser = getCurrentUsername();
        int rows = userTenantMapper.softDeleteUserTenants(
                data.getTenantId(),
                data.getUsernames(),
                currentUser
        );
        return JsonVO.success("成功移除 " + rows + " 个用户");
    }

    private String getCurrentUsername() {
        try {
            return userHolder.getCurrentUser().getUsername();
        } catch (Exception e) {
            return "system";
        }
    }
}
