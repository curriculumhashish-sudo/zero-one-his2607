package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import org.apache.ibatis.annotations.Param;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>
 * 工作流 inventory item Mapper 接口
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {
    int updateQuantityForStocktake(@Param("itemTable") String itemTable,
                                   @Param("itemId") String itemId,
                                   @Param("locationId") String locationId,
                                   @Param("lotNumber") String lotNumber,
                                   @Param("unitCode") String unitCode,
                                   @Param("quantity") BigDecimal quantity,
                                   @Param("updatedAt") LocalDateTime updatedAt,
                                   @Param("updatedBy") String updatedBy);

}
