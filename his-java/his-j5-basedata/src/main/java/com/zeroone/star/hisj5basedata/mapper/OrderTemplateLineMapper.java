package com.zeroone.star.hisj5basedata.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.hisj5basedata.entity.OrderTemplateLine;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;

/**
 * <p>
 * 组套模板行 Mapper 接口
 * </p>
 * @author keepasdd
 */
@Mapper
public interface OrderTemplateLineMapper extends BaseMapper<OrderTemplateLine> {

    /**
     * 通过模板头 ID 查询明细行
     * @param orderTemplateId 组套模板头 ID
     * @return 明细行列表
     */
    List<OrderTemplateLine> selectByTemplateId(String orderTemplateId);
}
