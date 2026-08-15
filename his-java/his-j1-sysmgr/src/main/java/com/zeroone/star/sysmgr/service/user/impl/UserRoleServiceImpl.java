package com.zeroone.star.sysmgr.service.user.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.zeroone.star.sysmgr.entity.role.RoleDO;
import com.zeroone.star.sysmgr.entity.user.UserRole;
import com.zeroone.star.sysmgr.mapper.role.RoleMapper;
import com.zeroone.star.sysmgr.mapper.user.UserRoleMapper;
import com.zeroone.star.sysmgr.service.user.IUserRoleService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.*;
import java.util.stream.Collectors;

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

    @Resource
    private RoleMapper roleMapper;

    @Override
    public Map<String, List<RoleInfo>> getRoleInfoByUserIds(List<String> userIds) {
        if (userIds == null || userIds.isEmpty()) {
            return Collections.emptyMap();
        }
        // 1.查用户角色关联表
        List<UserRole> userRoles = baseMapper.selectList(
                new LambdaQueryWrapper<UserRole>().in(UserRole::getUserId, userIds));
        // 2.收集所有角色ID，批量查角色表
        Set<String> roleIds = userRoles.stream().map(UserRole::getRoleId).collect(Collectors.toSet());
        Map<String, RoleDO> roleMap = roleIds.isEmpty()
                ? Collections.emptyMap()
                : roleMapper.selectBatchIds(roleIds).stream()
                        .collect(Collectors.toMap(RoleDO::getId, r -> r));
        // 3.按 userId 分组
        return userRoles.stream().collect(Collectors.groupingBy(
                UserRole::getUserId,
                Collectors.mapping(ur -> {
                    RoleDO role = roleMap.get(ur.getRoleId());
                    return new RoleInfo(ur.getRoleId(), role != null ? role.getDisplayName() : "");
                }, Collectors.toList())
        ));
    }
}
