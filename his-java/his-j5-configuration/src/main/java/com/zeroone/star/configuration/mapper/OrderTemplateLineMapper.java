package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.entity.OrderTemplateLine;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 组套模板行 Mapper 接口 —— 所有查询显式列，禁止 SELECT *
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author keepasdd
 * @version 1.0.0
 */
@Mapper
public interface OrderTemplateLineMapper extends BaseMapper<OrderTemplateLine> {

    /**
     * 通过模板头 ID 查询明细行（按 sort_order 升序）
     * @param orderTemplateId 组套模板头 ID
     * @return 明细行列表（已排除软删行）
     */
    List<OrderTemplateLine> selectByTemplateId(@Param("orderTemplateId") String orderTemplateId);
}