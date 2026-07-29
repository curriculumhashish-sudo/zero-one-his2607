package com.zeroone.star.pharmacy.mapper;

import com.zeroone.star.pharmacy.entity.InStockOrder;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：入库单主表 Mapper 接口，对应数据库表 {@code pharmacy_in_order}
 * </p>
 * <p>
 * 功能说明：提供入库单主表的数据库操作能力，包括插入、更新、状态变更、查询、分页及统计。
 * 所有 SQL 映射定义在对应的 XML 文件中（{@code InStockOrderMapper.xml}）。
 * 在药房入库业务流程中，该 Mapper 负责入库单单据的管理及状态流转。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface InStockOrderMapper {

    /**
     * <p>
     * 插入一条入库单主表记录
     * </p>
     *
     * @param order 入库单实体（主键 id 由数据库自动生成并回填）
     * @return 影响行数
     */
    int insertOrder(InStockOrder order);

    /**
     * <p>
     * 根据主键 ID 动态更新入库单主表字段（主键不可修改）
     * </p>
     *
     * @param order 包含待更新字段的入库单实体，id 必须赋值
     * @return 影响行数
     */
    int updateOrderById(InStockOrder order);

    /**
     * <p>
     * 更新入库单的状态
     * </p>
     *
     * @param id     入库单主键 ID
     * @param status 状态码（0-草稿，1-待审批，2-已审批，3-入库完成）
     * @return 影响行数
     */
    int updateOrderStatus(@Param("id") Long id, @Param("status") Integer status);

    /**
     * <p>
     * 根据主键 ID 查询入库单
     * </p>
     *
     * @param id 入库单主键 ID
     * @return 入库单实体，未找到时返回 null
     */
    InStockOrder selectOrderById(@Param("id") Long id);

    /**
     * <p>
     * 根据入库单号查询入库单
     * </p>
     *
     * @param orderNo 入库单号（唯一）
     * @return 入库单实体，未找到时返回 null
     */
    InStockOrder selectOrderByOrderNo(@Param("orderNo") String orderNo);

    /**
     * <p>
     * 分页查询入库单列表，按创建时间降序排列
     * </p>
     *
     * @param offset 起始偏移量
     * @param limit  每页条数
     * @return 入库单列表
     */
    List<InStockOrder> selectOrdersPage(@Param("offset") int offset, @Param("limit") int limit);

    /**
     * <p>
     * 统计入库单总条数
     * </p>
     *
     * @return 入库单总数
     */
    Long countOrders();

    /**
     * <p>
     * 物理删除入库单（一般不建议使用，实际应逻辑删除）
     * </p>
     *
     * @param id 入库单主键 ID
     * @return 影响行数
     */
    int deleteOrderById(@Param("id") Long id);
}