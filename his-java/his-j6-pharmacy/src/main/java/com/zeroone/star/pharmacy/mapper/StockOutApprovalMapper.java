package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;

/**
 * 药库出库 Mapper
 * 操作 wkf_supply_request 表，type_enum = WAREHOUSE_STOCKOUT
 */
public interface StockOutApprovalMapper extends BaseMapper<WkfSupplyRequest> {
}
