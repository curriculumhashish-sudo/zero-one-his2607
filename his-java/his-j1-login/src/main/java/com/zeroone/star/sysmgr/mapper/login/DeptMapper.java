package com.zeroone.star.sysmgr.mapper.login;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.login.WhaleOrganizationUnitDO;
import org.apache.ibatis.annotations.Param;

import java.util.List;

public interface DeptMapper extends BaseMapper<WhaleOrganizationUnitDO> {
    List<WhaleOrganizationUnitDO> selectUserDeptList(@Param("userId") String userId, @Param("tenantId") String tenantId);
}
