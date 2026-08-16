package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyDelivery;
import com.zeroone.star.pharmacy.service.impl.WkfSupplyRequestServiceImpl;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 工作流 supply delivery Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface WkfSupplyDeliveryMapper extends BaseMapper<WkfSupplyDelivery> {

    // WkfSupplyDeliveryMapper
    List<WkfSupplyRequestServiceImpl.RequestSummaryDTO> selectSummaryByRequestIds(@Param("ids") List<String> ids);

}
