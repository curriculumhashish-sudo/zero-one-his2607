package com.zeroone.star.sysmgr.service.user;

import com.zeroone.star.sysmgr.entity.user.UserOrganizationUnit;
import com.baomidou.mybatisplus.extension.service.IService;

/**
 * <p>
 * 用户组织单元关联 服务类
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-25
 */
public interface IUserOrganizationUnitService extends IService<UserOrganizationUnit> {

    /**
     *
     * @param userId
     * @param defaultOrgId
     */
    void add(String userId, String defaultOrgId);
}
