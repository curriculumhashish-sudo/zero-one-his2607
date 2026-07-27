package com.zeroone.star.sysmgr.mapper.role;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.role.RoleDO;
import org.apache.ibatis.annotations.Param;

/**
 * 角色数据访问接口
 * <p>
 * 提供角色查询、基本信息更新、引用检查和逻辑删除操作
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
public interface RoleMapper extends BaseMapper<RoleDO> {

    /**
     * 统计内部角色名称，包括已逻辑删除的数据。
     *
     * @param name 内部角色名称
     * @return 角色数量
     */
    long countByName(@Param("name") String name);

    /**
     * 修改角色基本信息。
     *
     * @param roleId 角色编号
     * @param tenantId 当前租户编号
     * @param displayName 角色显示名称
     * @param description 角色描述
     * @param isDefault 是否为默认角色
     * @param updatedBy 修改人编号
     * @return 影响行数
     */
    int updateBasicInfo(@Param("roleId") String roleId,
                        @Param("tenantId") String tenantId,
                        @Param("displayName") String displayName,
                        @Param("description") String description,
                        @Param("isDefault") Boolean isDefault,
                        @Param("updatedBy") String updatedBy);

    /**
     * 统计角色的有效用户关联。
     *
     * @param roleId 角色编号
     * @return 关联数量
     */
    long countActiveUserRoles(@Param("roleId") String roleId);

    /**
     * 统计角色的有效岗位关联。
     *
     * @param roleId 角色编号
     * @return 关联数量
     */
    long countActivePositionAssignments(@Param("roleId") String roleId);

    /**
     * 逻辑删除角色的授权记录。
     *
     * @param roleId 角色编号
     * @param deletedBy 删除人编号
     * @return 影响行数
     */
    int softDeletePermissionGrants(@Param("roleId") String roleId,
                                   @Param("deletedBy") String deletedBy);

    /**
     * 逻辑删除当前租户的非内置角色。
     *
     * @param roleId 角色编号
     * @param tenantId 租户编号
     * @param deletedBy 删除人编号
     * @return 影响行数
     */
    int softDeleteRole(@Param("roleId") String roleId,
                       @Param("tenantId") String tenantId,
                       @Param("deletedBy") String deletedBy);
}
