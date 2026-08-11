package com.zeroone.star.pharmacy.mapper;

import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * <p>
 * 描述：入库单明细 Mapper 接口，明细行存储于 {@code wkf_supply_request}（同表多行，按 bus_no 聚合）
 * </p>
 * <p>
 * 功能说明：提供入库单明细的插入、查询、软删除能力。
 * 插入时从主表行复制必填的表头字段；查询时关联 med_medication_definition 回显药品信息。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Mapper
public interface InStockOrderDetailMapper {

    /**
     * 插入一条入库明细行（写入 wkf_supply_request，表头字段从主表行复制）
     * @param orderId 入库单主表行 ID
     * @param detail  明细数据
     * @return 受影响行数
     */
    int insertDetail(@Param("orderId") String orderId, @Param("detail") InStockOrderDetail detail);

    /**
     * 根据主表行 ID 查询全部明细（关联药品定义表回显名称/规格/厂家）
     * @param orderId 入库单主表行 ID
     * @return 明细列表（按插入顺序）
     */
    List<InStockOrderDetail> selectDetailsByOrderId(@Param("orderId") String orderId);

    /**
     * 根据主表行 ID 软删除全部明细行
     * @param orderId   入库单主表行 ID
     * @param deletedBy 删除人
     * @param deletedAt 删除时间
     * @return 受影响行数
     */
    int softDeleteDetailsByOrderId(@Param("orderId") String orderId,
                                   @Param("deletedBy") String deletedBy,
                                   @Param("deletedAt") String deletedAt);
}