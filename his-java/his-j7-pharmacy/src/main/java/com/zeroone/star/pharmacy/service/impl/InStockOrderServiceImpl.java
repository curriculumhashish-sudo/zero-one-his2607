package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import com.zeroone.star.pharmacy.mapper.InStockOrderDetailMapper;
import com.zeroone.star.pharmacy.mapper.InStockOrderMapper;
import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDetailDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：入库单服务实现类
 * </p>
 * <p>
 * 功能说明：实现药房入库单的新增、草稿保存、保存并提交审批等核心业务功能。
 * 涉及的表包括：
 * - wkf_supply_request（供应请求主表）：充当入库单主表，管理状态与总金额。
 * - pharmacy_in_item（入库单明细表）：存储药品明细并参与汇总。
 * 在业务流程中，系统自动计算行合计金额和整单总金额，并严格控制状态流转
 * （草稿 DRAFT → 待审批 PENDING → 已审批 APPROVED → 入库完成 COMPLETED）。
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
    private InStockOrderDetailMapper detailMapper;

    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING = "PENDING";

    @Override
    @Transactional
    public String create(InStockOrderDTO dto) {
        InStockOrder order = buildOrder(dto);
        order.setStatusEnum(STATUS_DRAFT);
        order.setTotalPrice(BigDecimal.ZERO);
        orderMapper.insert(order);

        saveItems(order.getId(), dto.getItems());
        updateTotalPrice(order.getId());
        return order.getId();
    }

    @Override
    @Transactional
    public void update(String orderId, InStockOrderDTO dto) {
        InStockOrder existOrder = orderMapper.selectById(orderId);
        if (existOrder == null) {
            throw new RuntimeException("入库单不存在");
        }
        if (!STATUS_DRAFT.equals(existOrder.getStatusEnum())) {
            throw new RuntimeException("只有草稿状态才能编辑");
        }

        InStockOrder updateOrder = buildOrder(dto);
        updateOrder.setId(orderId);
        orderMapper.updateById(updateOrder);

        // 明细全量替换
        detailMapper.delete(new LambdaQueryWrapper<InStockOrderDetail>()
                .eq(InStockOrderDetail::getPharmacyId, orderId));
        saveItems(orderId, dto.getItems());

        updateTotalPrice(orderId);
    }

    @Override
    @Transactional
    public void submit(String orderId, InStockOrderDTO dto) {
        update(orderId, dto);

        int rows = orderMapper.update(null,
                new LambdaUpdateWrapper<InStockOrder>()
                        .eq(InStockOrder::getId, orderId)
                        .eq(InStockOrder::getStatusEnum, STATUS_DRAFT)
                        .set(InStockOrder::getStatusEnum, STATUS_PENDING));

        if (rows == 0) {
            throw new RuntimeException("当前状态不允许提交审批");
        }
    }

    // ==================== 私有方法 ====================

    private InStockOrder buildOrder(InStockOrderDTO dto) {
        InStockOrder order = new InStockOrder();
        order.setBusNo(dto.getOrderNo());
        order.setSupplierId(String.valueOf(dto.getSupplierId()));
        order.setOccurrenceTime(dto.getInDate() != null ?
                dto.getInDate().atStartOfDay() : LocalDateTime.now());
        order.setPurposeLocationId(String.valueOf(dto.getWarehouseId()));
        order.setRemake(dto.getRemark());
        // 设置入库类型和初始状态
        order.setTypeEnum("INVENTORY_IN");
        return order;
    }

    private void saveItems(String orderId, List<InStockOrderDetailDTO> itemDTOs) {
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
            item.setTotalAmount(dto.getUnitPrice().multiply(BigDecimal.valueOf(dto.getQuantity())));
            item.setBatchNo(dto.getBatchNo());
            item.setExpireDate(dto.getExpireDate());
            item.setItemRemark(dto.getItemRemark());
            items.add(item);
        }
        for (InStockOrderDetail item : items) {
            detailMapper.insert(item);
        }
    }

    private void updateTotalPrice(String orderId) {
        List<InStockOrderDetail> items = detailMapper.selectList(
                new LambdaQueryWrapper<InStockOrderDetail>()
                        .eq(InStockOrderDetail::getPharmacyId, orderId));
        BigDecimal total = items.stream()
                .map(InStockOrderDetail::getTotalAmount)
                .reduce(BigDecimal.ZERO, BigDecimal::add);

        InStockOrder order = new InStockOrder();
        order.setId(orderId);
        order.setTotalPrice(total);
        orderMapper.updateById(order);
    }
}