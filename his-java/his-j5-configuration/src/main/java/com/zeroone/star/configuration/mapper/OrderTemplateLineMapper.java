package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.OrderTemplateLine;
import org.apache.ibatis.annotations.Mapper;

/**
 * <p>
 * 组套模板行 Mapper 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Mapper
public interface OrderTemplateLineMapper extends BaseMapper<OrderTemplateLine> {
}