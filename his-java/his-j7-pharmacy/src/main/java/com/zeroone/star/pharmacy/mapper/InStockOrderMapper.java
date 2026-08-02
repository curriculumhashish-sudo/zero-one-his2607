package com.zeroone.star.pharmacy.mapper;

import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：供应请求主表 Mapper 接口，对应数据库表 {@code wkf_supply_request}
 * </p>
 * <p>
 * 功能说明：提供供应请求主表的数据库操作能力，包括插入、更新、删除、查询等。
 * 继承 MyBatis-Plus 的 BaseMapper，无需手动编写 SQL 即可完成单表 CRUD。
 * 在药房入库业务流程中，通过该 Mapper 维护入库单的状态并更新整单总金额。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface InStockOrderMapper extends BaseMapper<InStockOrder> {
    int insertOrder(InStockOrder order);
    int updateOrderById(InStockOrder order);
    int updateOrderStatus(@Param("id") String id, @Param("statusEnum") String statusEnum);
    InStockOrder selectOrderById(@Param("id") String id);
    InStockOrder selectOrderByBusNo(@Param("busNo") String busNo);
    List<InStockOrder> selectOrdersPage(@Param("offset") int offset, @Param("limit") int limit);
    Long countOrders();

    /**
     * 根据 bus_no 软删除入库单（设置 deleted_at、deleted_by）
     */
    int softDeleteByBusNo(@Param("busNo") String busNo,
                          @Param("deletedBy") String deletedBy,
                          @Param("deletedAt") String deletedAt);
}