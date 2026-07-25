package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import org.apache.ibatis.annotations.Mapper;

/**
 * 库存项 Mapper
 * 对应表：wkf_inventory_item
 *
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {
}
