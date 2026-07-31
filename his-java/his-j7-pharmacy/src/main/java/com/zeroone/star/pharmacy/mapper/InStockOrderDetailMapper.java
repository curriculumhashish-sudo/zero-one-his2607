package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.math.BigDecimal;
import java.util.List;

/**
 * <p>
 * 描述：入库单明细表 Mapper 接口，对应数据库表 {@code pharmacy_in_item}
 * </p>
 * <p>
 * 功能说明：提供入库单明细的数据库操作能力，包括插入、更新、删除、查询等。
 * 继承 MyBatis-Plus 的 BaseMapper，可直接使用单表 CRUD 方法。
 * 在药房入库业务流程中，该 Mapper 负责管理单条入库单的药品明细数据。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface InStockOrderDetailMapper extends BaseMapper<InStockOrderDetail> {
    int insertOrderDetail(InStockOrderDetail detail);
    int insertBatchOrderDetail(@Param("items") List<InStockOrderDetail> items);
    int updateOrderDetailById(InStockOrderDetail detail);
    int deleteOrderDetailsByOrderId(@Param("orderId") String orderId);
    List<InStockOrderDetail> selectOrderDetailsByOrderId(@Param("orderId") String orderId);
    Long countOrderDetailsByOrderId(@Param("orderId") String orderId);
    BigDecimal sumAmountByOrderId(@Param("orderId") String orderId);
}