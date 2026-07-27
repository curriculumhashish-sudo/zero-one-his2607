package com.zeroone.star.sysmgr.service.user;

import com.zeroone.star.sysmgr.entity.user.UserRole;
import com.baomidou.mybatisplus.extension.service.IService;
import jakarta.validation.constraints.NotEmpty;

import java.util.List;

/**
 * <p>
 * 用户角色关联 服务类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
public interface IUserRoleService extends IService<UserRole> {

    /**
     *
     * @param userId
     * @param roleIds
     */
    void add(String userId, @NotEmpty(message = "角色不能为空") List<String> roleIds);
}
