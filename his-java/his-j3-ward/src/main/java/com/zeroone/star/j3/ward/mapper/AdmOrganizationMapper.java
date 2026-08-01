package com.zeroone.star.j3.ward.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.ward.entity.AdmOrganization;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 描述：管理域机构 Mapper
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
@Mapper
public interface AdmOrganizationMapper extends BaseMapper<AdmOrganization> {

    Long getTenantIdByUserId(Long userId);
}