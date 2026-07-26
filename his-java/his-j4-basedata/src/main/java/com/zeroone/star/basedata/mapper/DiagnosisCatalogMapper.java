package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.basedata.entity.diagnosis.TerConceptDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * 诊断目录分类数据访问接口。
 */
@Mapper
public interface DiagnosisCatalogMapper extends BaseMapper<TerConceptDO> {
}
