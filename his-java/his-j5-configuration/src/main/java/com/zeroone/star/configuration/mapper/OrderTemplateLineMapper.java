package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.OrderTemplateLine;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

@Mapper
public interface OrderTemplateLineMapper extends BaseMapper<OrderTemplateLine> {

    List<OrderTemplateLine> selectByTemplateId(String orderTemplateId);
}
