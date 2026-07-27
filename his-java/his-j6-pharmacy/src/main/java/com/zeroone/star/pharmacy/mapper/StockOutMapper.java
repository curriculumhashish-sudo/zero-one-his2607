package com.zeroone.star.pharmacy.mapper;

import com.zeroone.star.project.dto.j6.pharmacy.stockout.StockOutSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.stockout.StockOutDetailVO;
import org.apache.ibatis.annotations.Mapper;

/**
 * 出库单 Mapper
 */
@Mapper
public interface StockOutMapper {

    /**
     * 插入出库单
     */
    int insertStockOut(StockOutSaveDTO dto);

    /**
     * 更新出库单
     */
    int updateStockOut(StockOutSaveDTO dto);

    /**
     * 更新状态
     */
    int updateStatus(String busNo, String status);

    /**
     * 查询详情
     */
    StockOutDetailVO selectByBusNo(String busNo);
}