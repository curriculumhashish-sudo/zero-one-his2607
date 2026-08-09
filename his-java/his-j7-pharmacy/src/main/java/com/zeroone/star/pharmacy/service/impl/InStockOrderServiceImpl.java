package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.InStockOrder;
import com.zeroone.star.pharmacy.entity.InStockOrderDetail;
import com.zeroone.star.pharmacy.mapper.InStockOrderDetailMapper;
import com.zeroone.star.pharmacy.mapper.InStockOrderMapper;
import com.zeroone.star.pharmacy.mapper.InventoryLockMapper;
import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderCreateDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDetailDTO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderSaveDTO;
import com.zeroone.star.project.dto.j7.pharmacy.PurchaseInboundSubmitDTO;
import com.zeroone.star.project.query.j7.pharmacy.PurchaseInboundQuery;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundDetailVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PurchaseInboundListVO;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDeleteDTO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

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
@Slf4j
@Service
public class InStockOrderServiceImpl implements InStockOrderService {

    @Resource
    private InStockOrderMapper inStockOrderMapper;

    @Resource
    private InStockOrderDetailMapper detailMapper;

    @Resource
    private InventoryLockMapper inventoryLockMapper;

    @Resource
    private UserHolder userHolder;

    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING = "PENDING";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String CATEGORY_ENUM_PURCHASE_IN = "PURCHASE_IN";
    private static final String ITEM_TABLE_PHARMACY_IN = "pharmacy_in_item";
    private static final DateTimeFormatter BUS_NO_FORMATTER =
            DateTimeFormatter.ofPattern("yyyyMMddHHmmssSSS");

    private static final List<String> DELETABLE_STATUSES = List.of(STATUS_DRAFT, STATUS_PENDING_APPROVAL);
    private static final DateTimeFormatter DATE_TIME_FORMATTER =
            DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

    /**
     * <p>
     * 获取单据列表（条件+分页）
     * </p>
     *
     * @author chunmu
     */
    @Override
    public IPage<PurchaseInboundListVO> listInboundOrders(PurchaseInboundQuery query) {
        PurchaseInboundQuery safeQuery = query == null ? new PurchaseInboundQuery() : query;
        Page<PurchaseInboundListVO> page = new Page<>(
                normalizePageIndex(safeQuery.getPageIndex()),
                normalizePageSize(safeQuery.getPageSize()));

        return inStockOrderMapper.selectPurchaseInboundPage(
                page,
                safeQuery.getBusNo(),
                safeQuery.getSourceLocationId(),
                safeQuery.getPurposeLocationId(),
                safeQuery.getStatusEnum(),
                safeQuery.getStartTime(),
                safeQuery.getEndTime());
    }

    /**
     * <p>
     * 获取单据详情
     * </p>
     *
     * @author chunmu
     */
    @Override
    public PurchaseInboundDetailVO getInboundDetail(String id) {
        InStockOrder order = inStockOrderMapper.selectById(id);
        if (order == null) {
            throw new IllegalArgumentException("入库单不存在");
        }

        PurchaseInboundDetailVO detailVO = new PurchaseInboundDetailVO();
        detailVO.setId(order.getId());
        detailVO.setBusNo(order.getBusNo());
        detailVO.setSupplierId(order.getSupplierId());
        detailVO.setPurposeLocationId(order.getPurposeLocationId());
        detailVO.setTotalPrice(order.getTotalPrice());
        detailVO.setStatusEnum(order.getStatusEnum());
        detailVO.setApplyTime(order.getApplyTime());
        detailVO.setRemake(order.getRemake());

        List<InStockOrderDetail> detailItems = detailMapper.selectList(
                new LambdaQueryWrapper<InStockOrderDetail>()
                        .eq(InStockOrderDetail::getPharmacyId, order.getId())
                        .orderByAsc(InStockOrderDetail::getSeqNo));

        List<PurchaseInboundItemVO> itemVOs = detailItems.stream().map(item -> {
            PurchaseInboundItemVO itemVO = new PurchaseInboundItemVO();
            itemVO.setId(String.valueOf(item.getId()));
            itemVO.setItemId(item.getItemId());
            itemVO.setLotNumber(item.getBatchNo());
            itemVO.setQuantity(item.getQuantity() != null ?
                    BigDecimal.valueOf(item.getQuantity()) : null);
            itemVO.setUnit(null);
            itemVO.setPrice(item.getUnitPrice());
            itemVO.setSubTotal(item.getTotalAmount());
            return itemVO;
        }).collect(Collectors.toList());

        detailVO.setItems(itemVOs);
        return detailVO;
    }

    /**
     * <p>
     * 保存并提交审批
     * </p>
     *
     * @author chunmu
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public void submitInboundOrder(PurchaseInboundSubmitDTO dto) {
        InStockOrder order = inStockOrderMapper.selectById(dto.getId());
        if (order == null) {
            throw new IllegalArgumentException("入库单不存在");
        }

        if (!"DRAFT".equals(order.getStatusEnum()) && !"REJECTED".equals(order.getStatusEnum())) {
            throw new IllegalArgumentException("当前单据状态不允许提交审批");
        }

        // 3. 更新单据状态及提交时间
        order.setStatusEnum("PENDING_APPROVAL");
        order.setApplyTime(LocalDateTime.now());
        if (StringUtils.hasText(dto.getRemark())) {
            order.setRemake(dto.getRemark());
        }
        inStockOrderMapper.updateById(order);

        // 4. TODO: 如果前端传了新的明细，这里需要处理明细的增删改逻辑
    }

    @Override
    @Transactional
    public InStockOrderSaveDTO create(InStockOrderCreateDTO dto) {
        LocalDateTime now = LocalDateTime.now();
        String userId = getCurrentUserId();

        InStockOrder order = buildOrder(dto);
        String busNo = StringUtils.hasText(dto.getOrderNo()) ?
                dto.getOrderNo() : "PSI" + now.format(BUS_NO_FORMATTER);
        order.setBusNo(busNo);
        order.setStatusEnum(STATUS_DRAFT);
        order.setTotalPrice(BigDecimal.ZERO);
        order.setCreatedAt(now);
        order.setUpdatedAt(now);
        order.setCreatedBy(userId);
        order.setUpdatedBy(userId);
        order.setIsDeleted(false);
        order.setVersion(0);
        if (order.getCategoryEnum() == null) {
            order.setCategoryEnum(CATEGORY_ENUM_PURCHASE_IN);
        }
        order.setItemTable(ITEM_TABLE_PHARMACY_IN);
        order.setItemQuantity(BigDecimal.ZERO);
        order.setItemId("0");
        order.setUnitCode("-");
        order.setApplicantId(userId);
        inStockOrderMapper.insert(order);

        saveItems(order.getId(), dto.getItems());
        updateTotalPrice(order.getId());

        InStockOrder savedOrder = inStockOrderMapper.selectById(order.getId());
        return toSaveDTO(savedOrder);
    }

    @Override
    @Transactional
    public void update(String orderId, InStockOrderSaveDTO dto) {
        InStockOrder existOrder = inStockOrderMapper.selectById(orderId);
        if (existOrder == null) {
            throw new IllegalArgumentException("入库单不存在");
        }
        if (!STATUS_DRAFT.equals(existOrder.getStatusEnum())) {
            throw new IllegalArgumentException("只有草稿状态才能编辑");
        }

        InStockOrder updateOrder = buildOrder(dto);
        updateOrder.setId(orderId);
        updateOrder.setUpdatedAt(LocalDateTime.now());
        updateOrder.setUpdatedBy(getCurrentUserId());
        inStockOrderMapper.updateById(updateOrder);

        // 明细全量替换
        detailMapper.delete(new LambdaQueryWrapper<InStockOrderDetail>()
                .eq(InStockOrderDetail::getPharmacyId, orderId));
        saveItems(orderId, dto.getItems());

        updateTotalPrice(orderId);
    }

    @Override
    @Transactional
    public void submitByOrderId(String orderId) {
        InStockOrder order = inStockOrderMapper.selectById(orderId);
        if (order == null) {
            throw new IllegalArgumentException("入库单不存在");
        }
        if (!STATUS_DRAFT.equals(order.getStatusEnum())
                && !STATUS_PENDING_APPROVAL.equals(order.getStatusEnum())) {
            throw new IllegalArgumentException("当前状态不允许提交审批");
        }
        order.setStatusEnum(STATUS_PENDING);
        order.setApplyTime(LocalDateTime.now());
        order.setUpdatedAt(LocalDateTime.now());
        order.setUpdatedBy(getCurrentUserId());
        inStockOrderMapper.updateById(order);
    }

    @Override
    @Transactional
    public void submit(String orderId, InStockOrderSaveDTO dto) {
        update(orderId, dto);

        int rows = inStockOrderMapper.update(null,
                new LambdaUpdateWrapper<InStockOrder>()
                        .eq(InStockOrder::getId, orderId)
                        .eq(InStockOrder::getStatusEnum, STATUS_DRAFT)
                        .set(InStockOrder::getStatusEnum, STATUS_PENDING));

        if (rows == 0) {
            throw new IllegalArgumentException("当前状态不允许提交审批");
        }
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SupplyDocumentDeleteVO deleteOrder(InStockOrderDeleteDTO dto) {
        String busNo = dto.getBusNo().trim();

        InStockOrder order = inStockOrderMapper.selectOne(
                new LambdaQueryWrapper<InStockOrder>()
                        .eq(InStockOrder::getBusNo, busNo)
                        .last("LIMIT 1"));
        if (order == null) {
            throw new IllegalArgumentException("入库单不存在：" + busNo);
        }
        if (!DELETABLE_STATUSES.contains(order.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "只有草稿或待审核状态的入库单可以删除，当前状态：" + order.getStatusEnum());
        }

        int unlockedLocks = 0;
        try {
            inventoryLockMapper.deleteByBusNo(busNo);
            unlockedLocks = 1;
        } catch (Exception e) {
            log.warn("删除库存锁定记录失败：busNo={}, error={}", busNo, e.getMessage());
        }

        detailMapper.delete(new LambdaQueryWrapper<InStockOrderDetail>()
                .eq(InStockOrderDetail::getPharmacyId, order.getId()));

        LocalDateTime now = LocalDateTime.now();
        String deletedBy = getCurrentUserId();
        int deletedLines = inStockOrderMapper.softDeleteByBusNo(
                busNo, deletedBy, now.format(DATE_TIME_FORMATTER));
        if (deletedLines == 0) {
            throw new IllegalArgumentException("删除入库单失败，单据可能已被其他操作修改：" + busNo);
        }

        log.info("入库单 {} 删除成功，删除 {} 行，解锁 {} 条锁定记录", busNo, deletedLines, unlockedLocks);

        SupplyDocumentDeleteVO vo = new SupplyDocumentDeleteVO();
        vo.setBusNo(busNo);
        vo.setDeletedLines(deletedLines);
        vo.setUnlockedLocks(unlockedLocks);
        return vo;
    }

    // ==================== 私有方法 ====================

    private InStockOrder buildOrder(InStockOrderCreateDTO dto) {
        InStockOrder order = new InStockOrder();
        order.setBusNo(dto.getOrderNo());
        order.setSourceLocationId(dto.getSourceLocationId());
        order.setPurposeLocationId(dto.getWarehouseId());
        order.setOccurrenceTime(dto.getInDate() != null ?
                dto.getInDate().atStartOfDay() : LocalDateTime.now());
        order.setSupplierId(dto.getSupplierId() != null ?
                String.valueOf(dto.getSupplierId()) : null);
        order.setRemake(dto.getRemark());
        order.setTypeEnum("INVENTORY_IN");
        order.setCategoryEnum(dto.getProjectType() != null ?
                dto.getProjectType() : CATEGORY_ENUM_PURCHASE_IN);
        return order;
    }

    private void saveItems(String orderId, List<InStockOrderDetailDTO> itemDTOs) {
        List<InStockOrderDetail> items = new ArrayList<>();
        for (InStockOrderDetailDTO dto : itemDTOs) {
            InStockOrderDetail item = new InStockOrderDetail();
            item.setPharmacyId(orderId);
            item.setSeqNo(dto.getSeqNo());
            item.setItemId(dto.getItemId());
            item.setDrugName(dto.getDrugName());
            item.setSpec(dto.getSpec());
            item.setManufacturer(dto.getManufacturer());
            item.setQuantity(dto.getQuantity());
            item.setUnitPrice(dto.getUnitPrice());
            item.setTotalAmount(dto.getUnitPrice().multiply(BigDecimal.valueOf(dto.getQuantity())));
            item.setBatchNo(dto.getBatchNo());
            item.setTraceNo(dto.getTraceNo());
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
        inStockOrderMapper.updateById(order);
    }

    private InStockOrderSaveDTO toSaveDTO(InStockOrder order) {
        InStockOrderSaveDTO dto = new InStockOrderSaveDTO();
        dto.setOrderNo(order.getBusNo());
        dto.setSourceLocationId(order.getSourceLocationId());
        dto.setWarehouseId(order.getPurposeLocationId());
        dto.setProjectType(order.getCategoryEnum());
        dto.setSupplierId(order.getSupplierId() != null ?
                Long.valueOf(order.getSupplierId()) : null);
        dto.setInDate(order.getOccurrenceTime() != null ?
                order.getOccurrenceTime().toLocalDate() : null);
        dto.setRemark(order.getRemake());
        dto.setId(order.getId());
        dto.setStatusEnum(order.getStatusEnum());
        dto.setCreatedBy(order.getCreatedBy());
        dto.setCreatedAt(order.getCreatedAt());
        dto.setApproverId(order.getApproverId());
        dto.setApprovalTime(order.getApprovalTime());
        dto.setTotalPrice(order.getTotalPrice());
        return dto;
    }

    private String getCurrentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            return user != null && user.getId() != null ? user.getId() : "system";
        } catch (Exception e) {
            return "system";
        }
    }

    private long normalizePageIndex(long pageIndex) {
        return pageIndex < 1 ? 1 : pageIndex;
    }

    private long normalizePageSize(long pageSize) {
        if (pageSize < 1) {
            return 10;
        }
        return Math.min(pageSize, 100);
    }
}
