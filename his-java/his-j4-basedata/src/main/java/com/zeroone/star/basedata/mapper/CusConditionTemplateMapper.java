package com.zeroone.star.diagnosis.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.diagnosis.entity.CusConditionTemplate;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 描述：诊断模板数据访问接口，对应 cus_condition_template 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface CusConditionTemplateMapper extends BaseMapper<CusConditionTemplate> {
}
