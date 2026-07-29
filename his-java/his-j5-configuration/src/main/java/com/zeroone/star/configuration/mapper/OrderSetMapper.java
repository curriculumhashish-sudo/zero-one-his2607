package com.zeroone.star.configuration.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.configuration.domain.OrderSetDO;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

/**
 * 组套模板聚合根数据访问层。
 */
@Mapper
public interface OrderSetMapper extends BaseMapper<OrderSetDO> {

    int updateContentByVersion(@Param("entity") OrderSetDO entity,
                               @Param("expectedVersion") Integer expectedVersion);

    int updateStatusByVersion(@Param("id") String id,
                              @Param("tenantId") String tenantId,
                              @Param("status") String status,
                              @Param("updatedBy") String updatedBy,
                              @Param("expectedVersion") Integer expectedVersion);

    int softDeleteByVersion(@Param("id") String id,
                            @Param("tenantId") String tenantId,
                            @Param("deletedBy") String deletedBy,
                            @Param("expectedVersion") Integer expectedVersion);
}
