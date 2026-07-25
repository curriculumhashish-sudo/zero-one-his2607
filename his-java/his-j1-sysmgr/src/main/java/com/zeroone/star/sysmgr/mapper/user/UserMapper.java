package com.zeroone.star.sysmgr.mapper.user;

import com.zeroone.star.sysmgr.entity.user.User;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import org.apache.ibatis.annotations.Param;

/**
 * <p>
 * 系统用户 Mapper 接口
 * </p>
 *
 * @author sykesolune
 * @since 2026-07-24
 */
public interface UserMapper extends BaseMapper<User> {

    /**
     * 软删除用户
     * @param id        用户ID
     * @param deletedBy 删除人
     * @return 影响行数
     */
    int softDeleteById(@Param("id") String id, @Param("deletedBy") String deletedBy);

    /**
     * 修改用户启用状态
     * @param id       用户ID
     * @param isActive 是否启用 (1:启用, 0:停用)
     * @return 影响行数
     */
    int updateStatus(@Param("id") String id, @Param("isActive") Integer isActive);

    /**
     * 锁定/解锁用户
     * @param id         用户ID
     * @param isLockedOut 是否锁定 (1:锁定, 0:解锁)
     * @return 影响行数
     */
    int lockUser(@Param("id") String id, @Param("isLockedOut") Integer isLockedOut);

    /**
     * 修改密码
     * @param id           用户ID
     * @param passwordHash 加密后的密码哈希
     * @return 影响行数
     */
    int updatePassword(@Param("id") String id, @Param("passwordHash") String passwordHash);
}
