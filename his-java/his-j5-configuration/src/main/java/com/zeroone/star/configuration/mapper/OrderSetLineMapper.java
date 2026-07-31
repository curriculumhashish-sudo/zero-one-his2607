package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.domain.OrderSetLineDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * 组套模板明细数据访问层。
 */
@Mapper
public interface OrderSetLineMapper extends BaseMapper<OrderSetLineDO> {

    int insertBatch(@Param("list") List<OrderSetLineDO> list);

    int updateContentByVersion(@Param("entity") OrderSetLineDO entity,
                               @Param("expectedVersion") Integer expectedVersion);

    int softDeleteByIds(@Param("ids") List<String> ids,
                        @Param("tenantId") String tenantId,
                        @Param("orderTemplateId") String orderTemplateId,
                        @Param("deletedBy") String deletedBy);

    int softDeleteByTemplateId(@Param("tenantId") String tenantId,
                               @Param("orderTemplateId") String orderTemplateId,
                               @Param("deletedBy") String deletedBy);
}
