package com.zeroone.stat.sysmgr.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.dto.j2.sysmgr.TenantUserDTO;
import com.zeroone.stat.sysmgr.entity.UserTenant;
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
