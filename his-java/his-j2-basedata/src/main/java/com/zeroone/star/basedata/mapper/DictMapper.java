package com.zeroone.star.basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;

import com.zeroone.star.basedata.entity.Dict;
import org.apache.ibatis.annotations.Mapper;
import java.util.List;
@Mapper
public interface DictMapper extends BaseMapper<Dict> {
    List<String> selectDictNameList();
}
