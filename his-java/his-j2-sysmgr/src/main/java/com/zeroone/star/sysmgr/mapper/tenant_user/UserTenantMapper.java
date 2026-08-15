package com.zeroone.star.sysmgr.mapper.tenant_user;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j2.sysmgr.tenant_userdto.TenantUserDTO;
import com.zeroone.star.sysmgr.entity.tenant_user.UserTenant;
import org.apache.ibatis.annotations.Param;

import java.util.List;

public interface UserTenantMapper extends BaseMapper<UserTenant> {

    IPage<TenantUserDTO> listTenantUsers(IPage<TenantUserDTO> page,
                                         @Param("tenantId") String tenantId,
                                         @Param("userName") String userName,
                                         @Param("name") String name);

    void addOrReactivateUserTenant(@Param("id") String id,
                                   @Param("userId") String userId,
                                   @Param("tenantId") String tenantId,
                                   @Param("createdBy") String createdBy,
                                   @Param("updatedBy") String updatedBy);

    int softDeleteUserTenants(@Param("tenantId") String tenantId,
                              @Param("usernames") List<String> usernames,
                              @Param("deletedBy") String deletedBy);
}
