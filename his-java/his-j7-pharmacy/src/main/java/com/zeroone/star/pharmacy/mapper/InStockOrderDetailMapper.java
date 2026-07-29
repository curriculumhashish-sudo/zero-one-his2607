package com.zeroone.star.pharmacy.mapper;

import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：入库单明细表 Mapper 接口，对应数据库表 {@code pharmacy_in_item}
 * </p>
 * <p>
 * 功能说明：提供入库单明细的数据库操作能力，包括插入、批量插入、更新、删除、查询及统计。
 * 所有 SQL 映射定义在对应的 XML 文件中（{@code InStockOrderDetailMapper.xml}）。
 * 在药房入库业务流程中，该 Mapper 负责管理单条入库单的药品明细数据。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface InStockOrderDetailMapper {

    /**
     * <p>
     * 插入单条入库单明细记录
     * </p>
     *
     * @param detail 入库单明细实体
     * @return 影响行数（同时 detail.id 会被回填）
     */
    int insertOrderDetail(InStockOrderDetail detail);

    /**
     * <p>
     * 批量插入入库单明细记录
     * </p>
     *
     * @param items 明细实体列表
     * @return 影响行数
     */
    int insertBatchOrderDetail(@Param("items") List<InStockOrderDetail> items);

    /**
     * <p>
     * 根据明细 ID 更新单条明细记录（动态更新非空字段）
     * </p>
     *
     * @param detail 包含更新字段的明细实体
     * @return 影响行数
     */
    int updateOrderDetailById(InStockOrderDetail detail);

    /**
     * <p>
     * 根据入库单 ID 删除该单所有明细记录
     * </p>
     *
     * @param orderId 入库单主表 ID
     * @return 影响行数
     */
    int deleteOrderDetailsByOrderId(@Param("orderId") Long orderId);

    /**
     * <p>
     * 根据入库单 ID 查询所有明细，按行号升序排列
     * </p>
     *
     * @param orderId 入库单主表 ID
     * @return 明细列表
     */
    List<InStockOrderDetail> selectOrderDetailsByOrderId(@Param("orderId") Long orderId);

    /**
     * <p>
     * 统计指定入库单下的明细行数
     * </p>
     *
     * @param orderId 入库单主表 ID
     * @return 明细数量
     */
    Long countOrderDetailsByOrderId(@Param("orderId") Long orderId);

    /**
     * <p>
     * 汇总指定入库单下所有明细的合计金额
     * </p>
     *
     * @param orderId 入库单主表 ID
     * @return 总金额（单位：元，返回值已通过 COALESCE 处理，不会为 null）
     */
    Integer sumAmountByOrderId(@Param("orderId") Long orderId);
}