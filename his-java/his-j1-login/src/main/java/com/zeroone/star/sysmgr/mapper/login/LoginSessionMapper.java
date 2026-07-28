package com.zeroone.star.sysmgr.mapper.login;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.login.LoginSessionDO;
import org.apache.ibatis.annotations.Insert;
import org.apache.ibatis.annotations.Param;
import org.apache.ibatis.annotations.Update;

/** 登录会话数据访问层。 */
public interface LoginSessionMapper extends BaseMapper<LoginSessionDO> {
    @Insert("""
            INSERT INTO whale_user_login_session (user_id, updated_at)
            VALUES (#{userId}, CURRENT_TIMESTAMP(6))
            ON DUPLICATE KEY UPDATE updated_at = CURRENT_TIMESTAMP(6)
            """)
    int touchByUserId(@Param("userId") String userId);


    @Update("UPDATE whale_user_login_session " +
            "SET organization_unit_id = #{orgUnitId}, updated_at = CURRENT_TIMESTAMP(6) " +
            "WHERE user_id = #{userId}")
    int updateSelectOrgUnitId(@Param("userId") String userId, @Param("orgUnitId") String orgUnitId);
}
