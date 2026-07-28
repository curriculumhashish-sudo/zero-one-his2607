package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.DocumentTypeDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * 文书类型Mapper
 */
@Mapper
public interface DocumentTypeMapper extends BaseMapper<DocumentTypeDO> {
}
