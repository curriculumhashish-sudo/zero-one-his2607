package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import com.zeroone.star.pharmacy.mapper.InStockOrderDetailMapper;
import com.zeroone.star.pharmacy.mapper.InStockOrderMapper;
import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDetailDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：入库单服务实现类
 * </p>
 * <p>
 * 功能说明：实现药房入库单的新增、草稿保存、保存并提交审批等核心业务功能。
 * 涉及的表包括：
 * - in_order（入库单主表）：入库单单据状态、总金额管理
 * - in_item（入库单明细表）：药品明细存储、金额汇总
 * 在业务流程中，系统自动计算行合计金额和整单总金额，并严格控制状态流转
 * （草稿→待审批→已审批→入库完成）。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class InStockOrderServiceImpl implements InStockOrderService {

    @Resource
    private InStockOrderMapper orderMapper;

    @Resource
    private InStockOrderDetailMapper orderDetailMapper;

    /**
     * 新增入库单
     * <p>
     * 创建一张新的入库单，初始状态为“草稿”（status=0），自动计算明细合计金额
     * 和整单总金额。入库单号可手动传入或由业务规则生成。本操作在事务中执行。
     * </p>
     *
     * @param dto 入库单数据传输对象，包含主表信息及药品明细列表
     * @return 新创建的入库单主键ID
     */
    @Override
    @Transactional
    public Long create(InStockOrderDTO dto) {
        // 1. 保存主表
        InStockOrder order = buildOrder(dto);
        order.setStatus(0); // 草稿
        order.setTotalAmount(BigDecimal.ZERO);
        orderMapper.insert(order);

        // 2. 保存明细
        saveItems(order.getId(), dto.getItems());

        // 3. 计算总金额并回写主表
        updateTotalAmount(order.getId());

        return order.getId();
    }

    /**
     * 保存入库单（编辑草稿）
     * <p>
     * 对已存在的草稿单据进行全量更新：主表字段覆盖、明细全量替换（先删后增），
     * 并重新计算整单总金额。仅允许状态为“草稿”的单据执行编辑。
     * </p>
     *
     * @param orderId 入库单主键ID
     * @param dto     更新后的入库单数据
     * @throws RuntimeException 如果单据不存在或状态不允许编辑
     */
    @Override
    @Transactional
    public void update(Long orderId, InStockOrderDTO dto) {
        // 1. 检查单据状态
        InStockOrder order = orderMapper.selectById(orderId);
        if (order == null) {
            throw new RuntimeException("入库单不存在");
        }
        if (order.getStatus() != 0) {
            throw new RuntimeException("只有草稿状态才能编辑");
        }

        // 2. 更新主表字段
        InStockOrder updateOrder = buildOrder(dto);
        updateOrder.setId(orderId);
        orderMapper.updateById(updateOrder);

        // 3. 明细全量替换：删除旧明细，插入新明细
        orderDetailMapper.delete(new LambdaQueryWrapper<InStockOrderDetail>()
                .eq(InStockOrderDetail::getPharmacyId, orderId));
        saveItems(orderId, dto.getItems());

        // 4. 重新计算总金额
        updateTotalAmount(orderId);
    }

    /**
     * 保存并提交审批
     * <p>
     * 在保存入库单内容后，将单据状态从“草稿”变更为“待审批”（status=1）。
     * 该操作要求单据当前必须处于草稿状态，否则抛出业务异常。
     * </p>
     *
     * @param orderId 入库单主键ID
     * @param dto     最终版本的入库单数据
     * @throws RuntimeException 如果单据状态不允许提交审批
     */
    @Override
    @Transactional
    public void submit(Long orderId, InStockOrderDTO dto) {
        // 1. 执行保存操作（内部已包含状态校验）
        update(orderId, dto);

        // 2. 使用乐观条件更新状态为“待审批”
        int rows = orderMapper.update(null,
                new LambdaUpdateWrapper<InStockOrder>()
                        .eq(InStockOrder::getId, orderId)
                        .eq(InStockOrder::getStatus, 0)     // 确保当前仍为草稿
                        .set(InStockOrder::getStatus, 1));  // 更新为待审批

        if (rows == 0) {
            throw new RuntimeException("当前状态不允许提交审批");
        }
    }

    // ==================== 私有辅助方法 ====================

    /**
     * 构建入库单主表实体
     */
    private InStockOrder buildOrder(InStockOrderDTO dto) {
        InStockOrder order = new InStockOrder();
        order.setOrderNo(dto.getOrderNo());
        order.setSupplierId(dto.getSupplierId());
        order.setInDate(dto.getInDate());
        order.setWarehouseId(dto.getWarehouseId());
        order.setRemark(dto.getRemark());
        return order;
    }

    /**
     * 批量保存明细，并自动计算每行合计金额
     */
    private void saveItems(Long orderId, List<InStockOrderDetailDTO> itemDTOs) {
        List<InStockOrderDetail> items = new ArrayList<>();
        for (InStockOrderDetailDTO dto : itemDTOs) {
            InStockOrderDetail item = new InStockOrderDetail();
            item.setPharmacyId(orderId);
            item.setSeqNo(dto.getSeqNo());
            item.setDrugName(dto.getDrugName());
            item.setSpec(dto.getSpec());
            item.setManufacturer(dto.getManufacturer());
            item.setQuantity(dto.getQuantity());
            item.setUnitPrice(dto.getUnitPrice());
            // 计算行合计金额
            item.setTotalAmount(dto.getUnitPrice().multiply(BigDecimal.valueOf(dto.getQuantity())));
            item.setBatchNo(dto.getBatchNo());
            item.setExpireDate(dto.getExpireDate());
            item.setItemRemark(dto.getItemRemark());
            items.add(item);
        }
        // 批量插入
        for (InStockOrderDetail item : items) {
            orderDetailMapper.insert(item);
        }
    }

    /**
     * 汇总明细总金额并更新主表
     */
    private void updateTotalAmount(Long orderId) {
        List<InStockOrderDetail> items = orderDetailMapper.selectList(
                new LambdaQueryWrapper<InStockOrderDetail>().eq(InStockOrderDetail::getPharmacyId, orderId));
        BigDecimal total = items.stream()
                .map(InStockOrderDetail::getTotalAmount)
                .reduce(BigDecimal.ZERO, BigDecimal::add);

        InStockOrder order = new InStockOrder();
        order.setId(orderId);
        order.setTotalAmount(total);
        orderMapper.updateById(order);
    }
}