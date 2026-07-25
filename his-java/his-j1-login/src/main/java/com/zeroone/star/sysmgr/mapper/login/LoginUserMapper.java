package com.zeroone.star.sysmgr.mapper.login;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.login.LoginUserDO;
import com.zeroone.star.sysmgr.entity.login.LoginUserProfileDO;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

/**
 * 登录用户数据访问层。
 */
public interface LoginUserMapper extends BaseMapper<LoginUserDO> {

    /**
     * 查询当前用户资料、登录选择和关联医护人员信息。
     */
    @Select("""
            SELECT u.id, u.user_name AS username, u.name, u.email, u.phone_number AS phoneNumber,
                   u.is_active AS isEnabled, u.is_locked_out AS lockedOut,
                   s.tenant_id AS tenantId, s.hospital_organization_id AS hospitalOrganizationId,
                   s.organization_unit_id AS organizationUnitId,
                   p.id AS practitionerId, p.name AS practitionerName, p.status_enum AS practitionerStatus
            FROM whale_users u
            LEFT JOIN whale_user_login_session s ON s.user_id = u.id
            LEFT JOIN adm_practitioner p ON p.user_id = u.id AND p.is_deleted = 0
            WHERE u.id = #{userId} AND u.is_deleted = 0
            """)
    LoginUserProfileDO selectProfileById(@Param("userId") String userId);

    @Select("SELECT id FROM whale_users WHERE user_name = #{username} AND is_deleted = 0")
    String selectIdByUsername(@Param("username") String username);
}
