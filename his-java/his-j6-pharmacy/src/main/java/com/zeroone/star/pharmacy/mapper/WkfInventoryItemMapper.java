package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import org.apache.ibatis.annotations.Param;

import java.math.BigDecimal;
import java.time.LocalDateTime;

/**
 * <p>工作流 inventory item Mapper 接口</p>
 *
 * @author AB
 * @since 2026-07-27
 */
public interface WkfInventoryItemMapper extends BaseMapper<WkfInventoryItem> {

    /**
     * 累加库存数量（按 itemTable + itemId + locationId + lotNumber + unitCode 五元组匹配已有行）。
     * 匹配到时 UPDATE quantity = quantity + #{quantity}，同时刷新 supplierId / expirationDate。
     * @return 1 表示更新成功，0 表示没有匹配行（调用方需 INSERT）
     */
    int addQuantity(@Param("itemTable") String itemTable,
                    @Param("itemId") String itemId,
                    @Param("locationId") String locationId,
                    @Param("lotNumber") String lotNumber,
                    @Param("unitCode") String unitCode,
                    @Param("quantity") BigDecimal quantity,
                    @Param("supplierId") String supplierId,
                    @Param("expirationDate") LocalDateTime expirationDate,
                    @Param("updatedAt") LocalDateTime updatedAt,
                    @Param("updatedBy") String updatedBy);
}
