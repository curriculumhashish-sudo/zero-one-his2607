package com.zeroone.star.sysmgr.mapper.login;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.login.LoginUserDO;
import com.zeroone.star.sysmgr.entity.login.LoginUserProfileDO;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Select;

import java.util.List;

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
                   s.tenant_id AS tenantId, t.name AS tenantName,
                   s.hospital_organization_id AS hospitalOrganizationId, h.name AS hospitalName,
                   s.organization_unit_id AS organizationUnitId, ou.display_name AS organizationUnitName,
                   p.id AS practitionerId, p.name AS practitionerName, p.status_enum AS practitionerStatus
            FROM whale_users u
            LEFT JOIN whale_user_login_session s ON s.user_id = u.id
            LEFT JOIN whale_tenants t ON t.id = s.tenant_id AND t.is_deleted = 0
            LEFT JOIN adm_organization h ON h.id = s.hospital_organization_id AND h.is_deleted = 0
            LEFT JOIN whale_organization_units ou ON ou.id = s.organization_unit_id AND ou.is_deleted = 0
            LEFT JOIN adm_practitioner p ON p.id = (
                SELECT p2.id FROM adm_practitioner p2
                WHERE p2.user_id = u.id AND p2.is_deleted = 0
                ORDER BY p2.id LIMIT 1
            )
            WHERE u.id = #{userId} AND u.is_deleted = 0
            ORDER BY s.updated_at DESC
            LIMIT 1
            """)
    LoginUserProfileDO selectProfileById(@Param("userId") String userId);

    /**
     * 同 selectProfileById，但返回 List 避免多行时 selectOne 报错
     */
    @Select("""
            SELECT u.id, u.user_name AS username, u.name, u.email, u.phone_number AS phoneNumber,
                   u.is_active AS isEnabled, u.is_locked_out AS lockedOut,
                   s.tenant_id AS tenantId, t.name AS tenantName,
                   s.hospital_organization_id AS hospitalOrganizationId, h.name AS hospitalName,
                   s.organization_unit_id AS organizationUnitId, ou.display_name AS organizationUnitName,
                   p.id AS practitionerId, p.name AS practitionerName, p.status_enum AS practitionerStatus
            FROM whale_users u
            LEFT JOIN whale_user_login_session s ON s.user_id = u.id
            LEFT JOIN whale_tenants t ON t.id = s.tenant_id AND t.is_deleted = 0
            LEFT JOIN adm_organization h ON h.id = s.hospital_organization_id AND h.is_deleted = 0
            LEFT JOIN whale_organization_units ou ON ou.id = s.organization_unit_id AND ou.is_deleted = 0
            LEFT JOIN adm_practitioner p ON p.id = (
                SELECT p2.id FROM adm_practitioner p2
                WHERE p2.user_id = u.id AND p2.is_deleted = 0
                ORDER BY p2.id LIMIT 1
            )
            WHERE u.id = #{userId} AND u.is_deleted = 0
            ORDER BY s.updated_at DESC
            LIMIT 1
            """)
    List<LoginUserProfileDO> selectProfileList(@Param("userId") String userId);

    @Select("SELECT id FROM whale_users WHERE user_name = #{username} AND is_deleted = 0")
    String selectIdByUsername(@Param("username") String username);
}
