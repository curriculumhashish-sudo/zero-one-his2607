package com.zeroone.star.j7.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.j7.pharmacy.entity.approval.WkfInventoryItem;
import org.apache.ibatis.annotations.Param;

import java.math.BigDecimal;
import java.util.List;

/**
 * <p>
 * 工作流 inventory item Mapper 接口
 * </p>
 *
 * @author tsfmn
 * @since 2026-07-22
 */
public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {

    /**
     * 根据聚合单号查询所有盘点单明细列表
     */
    List<WkfInventoryItem> findItemByBusNo(@Param("busNo") String busNo);

    /**
     * 根据itemId(药品id)、lotNumber(批号)、locationId(库房位置)查询库存项
     */
    WkfInventoryItem findByItemLotAndLocation(@Param("itemId") String itemId,
                                              @Param("lotNumber") String lotNumber,
                                              @Param("locationId") String locationId);

    /**
     * 更新库存数量（乐观锁）
     * @return 受影响行数，0表示版本冲突
     */
    int updateQuantity(@Param("id") String id,
                       @Param("delta") BigDecimal delta,
                       @Param("version") Integer version,
                       @Param("operatorId") String operatorId);
}
