package com.zeroone.star.sysmgr.mapper.tenant;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.sysmgr.entity.tenant.UserTenantDO;
import org.apache.ibatis.annotations.Mapper;

@Mapper
public interface UserTenantMapper extends BaseMapper<UserTenantDO> {
}