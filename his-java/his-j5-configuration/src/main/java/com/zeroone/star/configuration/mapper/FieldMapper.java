package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.EmrMetadataFieldDef;
import org.apache.ibatis.annotations.Mapper;

/**
 * 元数据字段增删改处理的Mapper
 * 作者：青梅不甜
 */
@Mapper
public interface FieldMapper extends BaseMapper<EmrMetadataFieldDef> {
}