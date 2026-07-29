package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.InventoryLock;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 库存锁定 Mapper 接口
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-27
 */
public interface InventoryLockMapper extends BaseMapper<InventoryLock> {

    /**
     * 检查库存项是否被锁定
     */
    List<InventoryLock> findByInventoryId(@Param("inventoryId") String inventoryId);

    /**
     * 根据业务单号删除所有锁定记录
     */
    void deleteByBusNo(@Param("busNo") String busNo);
}
