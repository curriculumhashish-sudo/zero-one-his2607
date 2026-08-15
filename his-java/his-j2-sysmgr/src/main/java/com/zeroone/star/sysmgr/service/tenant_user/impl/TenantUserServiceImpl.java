package com.zeroone.star.sysmgr.service.tenant_user.impl;

import cn.hutool.crypto.digest.BCrypt;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.AddTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.RemoveTenantUserDTO;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.TenantUserDTO;
import com.zeroone.star.project.query.j2.sysmgr.TenantUserQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.sysmgr.entity.tenant_user.User;
import com.zeroone.star.sysmgr.mapper.tenant_user.UserMapper;
import com.zeroone.star.sysmgr.mapper.tenant_user.UserTenantMapper;
import com.zeroone.star.sysmgr.service.tenant_user.ITenantUserService;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.stream.Collectors;
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

                query.getTenantId(),
                query.getUsername(),
                query.getName()
        );

        result.getRecords().forEach(dto -> {
            if (dto.getRoleNames() == null) {
                dto.setRoleNames(new ArrayList<>());
            }
        });
        return JsonVO.success(PageDTO.create((Page<TenantUserDTO>) result));
    }

    @Override
    public JsonVO<String> addTenantUsers(AddTenantUserDTO data) {
        if (data.getUsers() == null || data.getUsers().isEmpty()) {
            return JsonVO.fail("用户列表不能为空");
        }
        // 按用户名去重，保留首次出现的记录（对应 rent 模式中的重复检查）
        List<AddTenantUserDTO.TenantUserItem> dedupedUsers = data.getUsers().stream()
                .collect(Collectors.toMap(
                        AddTenantUserDTO.TenantUserItem::getUsername,
                        item -> item,
                        (existing, replacement) -> existing,
                        LinkedHashMap::new
                ))
                .values()
                .stream()
                .toList();
        String currentUser = getCurrentUsername();

        for (AddTenantUserDTO.TenantUserItem item : dedupedUsers) {
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
