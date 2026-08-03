package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.diagnosis.ClnConditionDefinitionDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 描述：诊断数据访问接口，对应 cln_condition_definition 表
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@Mapper
public interface DiagnosisMapper extends BaseMapper<ClnConditionDefinitionDO> {

    Page<String> selectDistinctSourceEnum(Page<String> page,
                                          @Param("tenantId") String tenantId,
                                          @Param("keyword") String keyword,
                                          @Param("sourceEnum") String sourceEnum);
}
