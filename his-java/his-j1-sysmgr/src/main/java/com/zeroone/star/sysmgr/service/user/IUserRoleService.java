package com.zeroone.star.sysmgr.service.user;

import com.zeroone.star.sysmgr.entity.user.UserRole;
import com.baomidou.mybatisplus.extension.service.IService;
import jakarta.validation.constraints.NotEmpty;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.util.List;
import java.util.Map;

/**
 * <p>
 * 用户角色关联 服务类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
public interface IUserRoleService extends IService<UserRole> {

    /** 用户角色信息 */
    @Data
    @AllArgsConstructor
    class RoleInfo {
        private String roleId;
        private String roleName;
    }

    /**
     * 新增用户角色关联
     * @param userId  用户ID
     * @param roleIds 角色ID列表
     */
    void add(String userId, @NotEmpty(message = "角色不能为空") List<String> roleIds);

    /**
     * 批量查询用户的角色信息
     * @param userIds 用户ID列表
     * @return key=用户ID, value=该用户的角色信息列表（含 roleId + roleName）
     */
    Map<String, List<RoleInfo>> getRoleInfoByUserIds(List<String> userIds);

}
