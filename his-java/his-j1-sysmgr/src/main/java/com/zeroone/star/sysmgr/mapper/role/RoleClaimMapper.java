package com.zeroone.star.sysmgr.mapper.role;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.role.RoleClaimDO;
import org.apache.ibatis.annotations.Param;

/**
 * 角色声明数据访问接口
 * <p>
 * 提供角色声明关联数据的持久化操作
 * </p>
 *
 * @author j1-保温杯热水仙人
 * @since 2026-07-24
 */
public interface RoleClaimMapper extends BaseMapper<RoleClaimDO> {

    /**
     * 逻辑删除指定角色的有效声明。
     *
     * @param roleId 角色编号
     * @param deletedBy 删除人编号
     * @return 影响行数
     */
    int softDeleteByRoleId(@Param("roleId") String roleId,
                           @Param("deletedBy") String deletedBy);
}
