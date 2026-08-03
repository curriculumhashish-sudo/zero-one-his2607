package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.baomidou.mybatisplus.core.toolkit.Constants;
import com.zeroone.star.basedata.entity.TerCodeSystem;
import org.apache.ibatis.annotations.Param;

public interface DictionaryTypeMapper extends BaseMapper<TerCodeSystem> {
    int countName(@Param("name") String name, @Param("excludeId") String excludeId);
    int countConcepts(@Param("dictionaryTypeId") String dictionaryTypeId);
    int updateDictionaryType(@Param(Constants.ENTITY) TerCodeSystem entity);
    int logicallyDeleteDictionaryType(@Param(Constants.ENTITY) TerCodeSystem entity);
}
