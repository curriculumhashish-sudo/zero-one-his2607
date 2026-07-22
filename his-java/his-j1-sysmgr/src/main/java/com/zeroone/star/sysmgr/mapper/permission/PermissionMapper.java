package com.zeroone.star.sysmgr.mapper.permission;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.permission.Permission;
import org.apache.ibatis.annotations.Param;
import java.util.List;

public interface PermissionMapper extends BaseMapper<Permission> {
    List<String> selectChildNames(@Param("parentName") String parentName,
                                  @Param("tenantId") String tenantId);
    String selectParentName(@Param("name") String name,
                            @Param("tenantId") String tenantId);
    int batchDeletePermissions(@Param("names") List<String> names,
                               @Param("deletedBy") String deletedBy,
                               @Param("tenantId") String tenantId);
    int batchDeleteGrants(@Param("permissionNames") List<String> permissionNames,
                          @Param("deletedBy") String deletedBy,
                          @Param("tenantId") String tenantId);
    long countByName(@Param("name") String name,
                     @Param("tenantId") String tenantId);
}