package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.EmrDocumentTypeDO;
import org.apache.ibatis.annotations.Mapper;

/**
 * 获取文书类型 Mapper
 * @author yjyl
 * @version 1.0.0
 */
@Mapper
public interface EmrDocumentTypeMapper extends BaseMapper<EmrDocumentTypeDO> {
}
