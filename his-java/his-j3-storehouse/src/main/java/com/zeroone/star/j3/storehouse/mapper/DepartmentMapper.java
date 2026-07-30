package com.zeroone.star.j3.storehouse.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j3.storehouse.entity.DepartmentDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * 科室 Mapper（查询 adm_organization 表）
 * @author mie
 * @version 1.0.0
 */
@Mapper
public interface DepartmentMapper extends BaseMapper<DepartmentDO> {
}
