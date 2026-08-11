package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
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
 * （编辑中 DRAFT → 待审批 PENDING_APPROVAL → 审批中 APPROVAL → 已通过 AGREE / 已驳回 REJECT / 已撤回 WITHDRAW）。
 * 同一 bus_no 下的主表行与明细行状态始终批量同步，避免列表重复展示。
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
    private static final String STATUS_APPROVAL = "APPROVAL";
    private static final String STATUS_REJECTED = "REJECTED";
    private static final String STATUS_REJECT = "REJECT";
    private static final String STATUS_WITHDRAWN = "WITHDRAWN";
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    private static final String CATEGORY_ENUM_PURCHASE_IN = "PURCHASE_IN";
    private static final String ITEM_TABLE_PHARMACY_IN = "pharmacy_in_item";
    private static final DateTimeFormatter BUS_NO_FORMATTER =
            DateTimeFormatter.ofPattern("yyyyMMddHHmmssSSS");

    /** 入库单全部 type_enum 取值（兼容新建单据与参考项目/测试数据）。 */
    private static final List<String> INBOUND_TYPE_ENUMS =
            List.of("CABINET_INBOUND_RECEIPT", "PHARMACY_STOCK_IN", "INVENTORY_IN");

    /** 可删除状态：编辑中、待审批；PENDING 为历史脏数据，同样允许清理。 */
    private static final List<String> DELETABLE_STATUSES =
            List.of(STATUS_DRAFT, STATUS_PENDING, STATUS_PENDING_APPROVAL);

    /** 可保存（编辑）状态：编辑中、待审批、已驳回（驳回后可编辑重新提交）。 */
    private static final List<String> EDITABLE_STATUSES =
            List.of(STATUS_DRAFT, STATUS_PENDING, STATUS_PENDING_APPROVAL,
                    STATUS_REJECTED, STATUS_REJECT);

    /** 可提交审批状态。 */
    private static final List<String> SUBMITTABLE_STATUSES =
            List.of(STATUS_DRAFT, STATUS_PENDING, STATUS_PENDING_APPROVAL,
                    STATUS_REJECTED, STATUS_REJECT, STATUS_WITHDRAWN, STATUS_WITHDRAW);

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
        detailVO.setSourceLocationId(order.getSourceLocationId());
        detailVO.setPurposeLocationId(order.getPurposeLocationId());
        detailVO.setInDate(order.getOccurrenceTime() != null ?
                order.getOccurrenceTime().toLocalDate() : null);
        detailVO.setProjectType(order.getCategoryEnum());
        detailVO.setTotalPrice(order.getTotalPrice());
        detailVO.setStatusEnum(order.getStatusEnum());
        detailVO.setApplyTime(order.getApplyTime());
        detailVO.setRemake(order.getRemake());
        detailVO.setCreateByName(order.getCreatedBy());
        detailVO.setApproverName(order.getApproverId());
        detailVO.setApproveTime(order.getApprovalTime());

        List<InStockOrderDetail> detailItems = detailMapper.selectDetailsByOrderId(order.getId());
        List<PurchaseInboundItemVO> itemVOs = detailItems.stream().map(item -> {
            PurchaseInboundItemVO itemVO = new PurchaseInboundItemVO();
            itemVO.setId(item.getId());
            itemVO.setItemId(item.getItemId());
            itemVO.setItemName(item.getDrugName());
            itemVO.setSpec(item.getSpec());
            itemVO.setManufacturer(item.getManufacturer());
            itemVO.setLotNumber(item.getBatchNo());
            itemVO.setQuantity(item.getQuantity() != null ?
                    BigDecimal.valueOf(item.getQuantity()) : null);
            itemVO.setUnit(null);
            itemVO.setPrice(item.getUnitPrice());
            itemVO.setSubTotal(item.getTotalAmount());
            itemVO.setTraceCode(item.getTraceNo());
            itemVO.setRemark(item.getItemRemark());
            itemVO.setExpirationDate(item.getExpireDate());
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

        if (!SUBMITTABLE_STATUSES.contains(order.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "当前单据状态不允许提交审批，当前状态：" + order.getStatusEnum());
        }

        if (StringUtils.hasText(dto.getRemark())) {
            InStockOrder remarkUpdate = new InStockOrder();
            remarkUpdate.setId(order.getId());
            remarkUpdate.setRemake(dto.getRemark());
            inStockOrderMapper.updateById(remarkUpdate);
        }

        // 提交审批：同一 bus_no 下主表与全部明细行整体流转为“审批中”，并写入申请时间
        int rows = changeStatusByBusNo(order.getBusNo(), STATUS_APPROVAL,
                LocalDateTime.now(), getCurrentUserId());
        if (rows == 0) {
            throw new IllegalArgumentException("提交审批失败，单据可能已被其他操作处理：" + order.getBusNo());
        }
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
        if (!EDITABLE_STATUSES.contains(existOrder.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "只有编辑中或待审批状态的入库单可以保存，当前状态：" + existOrder.getStatusEnum());
        }

        InStockOrder updateOrder = buildOrder(dto);
        updateOrder.setId(orderId);
        updateOrder.setUpdatedAt(LocalDateTime.now());
        updateOrder.setUpdatedBy(getCurrentUserId());
        inStockOrderMapper.updateById(updateOrder);

        // 明细全量替换
        detailMapper.softDeleteDetailsByOrderId(orderId, getCurrentUserId(),
                LocalDateTime.now().format(DATE_TIME_FORMATTER));
        saveItems(orderId, dto.getItems());

        updateTotalPrice(orderId);

        // 保存成功后单据进入“待审批”：主表行与新插入明细行按 bus_no 整体同步状态
        changeStatusByBusNo(existOrder.getBusNo(), STATUS_PENDING_APPROVAL, null, getCurrentUserId());
    }

    @Override
    @Transactional
    public void submitByOrderId(String orderId) {
        InStockOrder order = inStockOrderMapper.selectById(orderId);
        if (order == null) {
            throw new IllegalArgumentException("入库单不存在");
        }
        if (!SUBMITTABLE_STATUSES.contains(order.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "当前状态不允许提交审批，当前状态：" + order.getStatusEnum());
        }
        // 提交审批：同一 bus_no 下主表与全部明细行整体流转为“审批中”，并写入申请时间
        int rows = changeStatusByBusNo(order.getBusNo(), STATUS_APPROVAL,
                LocalDateTime.now(), getCurrentUserId());
        if (rows == 0) {
            throw new IllegalArgumentException("提交审批失败，单据可能已被其他操作处理：" + order.getBusNo());
        }
    }

    @Override
    @Transactional
    public void submit(String orderId, InStockOrderSaveDTO dto) {
        update(orderId, dto);

        InStockOrder order = inStockOrderMapper.selectById(orderId);
        if (order == null) {
            throw new IllegalArgumentException("入库单不存在");
        }
        int rows = changeStatusByBusNo(order.getBusNo(), STATUS_APPROVAL,
                LocalDateTime.now(), getCurrentUserId());
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
                    "只有编辑中或待审批状态的入库单可以删除，当前状态：" + order.getStatusEnum());
        }

        int unlockedLocks = 0;
        try {
            inventoryLockMapper.deleteByBusNo(busNo);
            unlockedLocks = 1;
        } catch (Exception e) {
            log.warn("删除库存锁定记录失败：busNo={}, error={}", busNo, e.getMessage());
        }

        LocalDateTime now = LocalDateTime.now();
        String deletedBy = getCurrentUserId();
        detailMapper.softDeleteDetailsByOrderId(order.getId(), deletedBy,
                now.format(DATE_TIME_FORMATTER));

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

    /**
     * 按 bus_no 批量流转整张单据（主表行 + 明细行）的状态，保证同单各行状态一致。
     *
     * @param busNo        业务单号
     * @param targetStatus 目标状态
     * @param applyTime    申请时间；为 null 时不更新 apply_time（仅提交审批时写入）
     * @param operatorId   操作人
     * @return 受影响行数
     */
    private int changeStatusByBusNo(String busNo, String targetStatus,
                                    LocalDateTime applyTime, String operatorId) {
        LambdaUpdateWrapper<InStockOrder> wrapper = new LambdaUpdateWrapper<InStockOrder>()
                .eq(InStockOrder::getBusNo, busNo)
                .in(InStockOrder::getTypeEnum, INBOUND_TYPE_ENUMS)
                .set(InStockOrder::getStatusEnum, targetStatus)
                .set(applyTime != null, InStockOrder::getApplyTime, applyTime)
                .set(InStockOrder::getUpdatedAt, LocalDateTime.now())
                .set(InStockOrder::getUpdatedBy, operatorId)
                .setSql("version = version + 1");
        return inStockOrderMapper.update(null, wrapper);
    }

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
        for (InStockOrderDetailDTO dto : itemDTOs) {
            InStockOrderDetail item = new InStockOrderDetail();
            item.setId(IdWorker.getIdStr());
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
            detailMapper.insertDetail(orderId, item);
        }
    }

    private void updateTotalPrice(String orderId) {
        List<InStockOrderDetail> items = detailMapper.selectDetailsByOrderId(orderId);
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
