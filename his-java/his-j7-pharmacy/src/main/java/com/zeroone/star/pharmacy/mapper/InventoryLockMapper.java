package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.InventoryLock;
import org.apache.ibatis.annotations.Mapper;

/**
 * 库存锁定 Mapper
 * 对应表：inventory_lock
 *
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface InventoryLockMapper extends BaseMapper<InventoryLock> {
}
