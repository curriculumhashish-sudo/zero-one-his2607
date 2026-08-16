package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyDelivery;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.StockOutApprovalMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.service.IStockoutApprovalService;
import com.zeroone.star.pharmacy.service.IWkfSupplyDeliveryService;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.AgreeStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.RejectStockOutDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.SubmitStockOutApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stockout.WithdrawStockOutApprovalDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.stream.Collectors;
import java.time.format.DateTimeFormatter;

/**
 * 药库出库审批 Service 实现
 */
@Service
public class StockOutApprovalServiceImpl implements IStockoutApprovalService {

    //
    private static final String TYPE_WAREHOUSE_STOCKOUT = "WAREHOUSE_STOCKOUT";
    //
    //待审核
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    //审核中
    private static final String STATUS_APPROVAL = "APPROVAL";
    //已撤回
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    //已通过
    private static final String STATUS_AGREE = "AGREE";
    //已驳回
    private static final String STATUS_REJECT = "REJECT";

    @Resource
    private StockOutApprovalMapper stockOutApprovalMapper;

    @Resource
    private WkfInventoryItemMapper inventoryItemMapper;

    @Resource
    private UserHolder userHolder;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean submitStockOutApproval(SubmitStockOutApprovalDTO dto) {
        // 1. 查询出库单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许提交审批（草稿/已撤回/已驳回）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL, STATUS_WITHDRAW, STATUS_REJECT);
        // 4. 更新状态为待审批
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        updateStatus(dto.getBusNo(), STATUS_APPROVAL, wrapper -> wrapper
                .set("applicant_id", userId)
                .set("apply_time", now)
                .set("approver_id", dto.getApproverId())//指定审批人id
                .set("approval_time", null)
                .set("reason", null)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean withdrawStockOutApproval(WithdrawStockOutApprovalDTO dto) {
        //1. 查询出库订单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许撤回审批（待审批）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_APPROVAL);
        //校验当前用户能否撤回该审批（applicantId 为 null 时跳过校验，兼容旧数据）
        String userId = currentUserId();
        String applicantId = rows.get(0).getApplicantId();
        if (applicantId != null && !applicantId.equals(userId)) {
            throw new IllegalStateException("当前用户不是该单据的申请人");
        }
        // 4. 更新状态为已撤回
        LocalDateTime now = LocalDateTime.now();
        updateStatus(dto.getBusNo(), STATUS_WITHDRAW, wrapper -> wrapper
                .set("approver_id",null)//清除审批人
                .set("approval_time",null)//清除审批时间
                .set("reason", null)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean agreeStockOut(AgreeStockOutDTO dto) {
        // 1. 查询出库单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许审批（待审批）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_APPROVAL);
        // 4. 审批人优先取 DTO 传入的值，否则用当前用户
        String userId = currentUserId();
        String approverId = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : userId;
        // 5. 更新状态为已批准
        LocalDateTime now = LocalDateTime.now();
        LocalDateTime approvalTime = resolveApprovalClock(dto.getApprovalClock());
        updateStatus(dto.getBusNo(), STATUS_AGREE, wrapper -> wrapper
                .set("approver_id", approverId)
                .set("approval_time", approvalTime)
                .set("updated_at", now)
                .set("updated_by", userId));

        // 6. 重新获取出库请求（状态更新后的最新数据）
        List<WkfSupplyRequest> approvedRows = requireWarehouseStockOut(dto.getBusNo());

        // 7. 创建出库配送记录(wkf_supply_delivery)
        createSupplyDelivery(approvedRows, dto.getContractBusNo(), now, userId);

        // 8. 源库库存扣减 + 目标药房库存增加(wkf_inventory_item)
        adjustInventoryForStockOut(approvedRows, now, userId);

        // 9. 生成药房入库申请单(wkf_supply_request)
        createPharmacyInboundRequest(approvedRows, dto.getContractBusNo(), now, userId);

        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean rejectStockOut(RejectStockOutDTO dto) {
        // 1. 查询出库单是否存在
        List<WkfSupplyRequest> rows = requireWarehouseStockOut(dto.getBusNo());
        // 2. 校验行状态一致
        requireSameStatus(rows);
        // 3. 校验当前状态允许提交审批（待审批）
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_APPROVAL);
        // 4. 审批人优先取 DTO 传入的值，否则用当前用户
        String userId = currentUserId();
        String approverId2 = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : userId;
        // 5. 更新状态为被驳回(写入驳回原因)
        LocalDateTime now = LocalDateTime.now();
        LocalDateTime approvalTime = resolveApprovalClock(dto.getApprovalClock());
        updateStatus(dto.getBusNo(), STATUS_REJECT, wrapper -> wrapper
                .set("approver_id", approverId2)
                .set("approval_time", approvalTime)
                .set("reason", dto.getRejectReason())//写入驳回原因
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    // ==================== 私有方法 ====================

    /**
     * 查询药库出库单（按 busNo + type_enum + 未删除）
     */
    private List<WkfSupplyRequest> requireWarehouseStockOut(String busNo) {
        List<WkfSupplyRequest> rows = stockOutApprovalMapper.selectList(new QueryWrapper<WkfSupplyRequest>()
                .eq("bus_no", busNo)
                .eq("type_enum", TYPE_WAREHOUSE_STOCKOUT)
                .eq("is_deleted", 0));
        if (rows.isEmpty()) {
            throw new IllegalArgumentException("药库出库单不存在: " + busNo);
        }
        return rows;
    }

    /**
     * 校验同一单据下的所有行状态一致
     */
    private void requireSameStatus(List<WkfSupplyRequest> rows) {
        Set<String> statuses = rows.stream()
                .map(WkfSupplyRequest::getStatusEnum)
                .collect(Collectors.toSet());
        if (statuses.size() != 1) {
            throw new IllegalStateException("出库单行状态不一致: " + statuses);
        }
    }

    /**
     * 校验当前状态是否在允许范围内
     */
    private void requireAnyStatus(String currentStatus, String... allowedStatuses) {
        for (String allowedStatus : allowedStatuses) {
            if (allowedStatus.equals(currentStatus)) {
                return;
            }
        }
        throw new IllegalStateException("当前状态 " + currentStatus + " 不允许执行此操作");
    }

    /**
     * 批量更新出库单状态
     */
    private void updateStatus(String busNo, String status, StatusUpdateCustomizer customizer) {
        UpdateWrapper<WkfSupplyRequest> wrapper = new UpdateWrapper<WkfSupplyRequest>()
                .eq("bus_no", busNo)
                .eq("type_enum", TYPE_WAREHOUSE_STOCKOUT)
                .eq("is_deleted", 0)
                .set("status_enum", status);
        customizer.customize(wrapper);
        int affectedRows = stockOutApprovalMapper.update(null, wrapper);
        if (affectedRows <= 0) {
            throw new IllegalStateException("更新出库审批状态失败");
        }
    }

    /**
     * 获取当前用户ID
     */
    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getId();
            }
        } catch (Exception ignored) {
            // 无网关 header 时保持可用（本地测试等场景）
        }
        return "system";
    }

    @Resource
    private IWkfSupplyDeliveryService supplyDeliveryService;

    /**
     * 创建出库配送记录（wkf_supply_delivery）
     */
    private void createSupplyDelivery(List<WkfSupplyRequest> rows, String contractBusNo, LocalDateTime now, String userId) {
        List<WkfSupplyDelivery> supplyDeliveryList = new ArrayList<>();
        // 遍历 rows，为每行创建一条配送记录，包含：
        //   - 出库单号(busNo)、物品信息(itemTable/itemId/unitCode/lotNumber)
        //   - 出库数量(itemQuantity)、源库位、目标库位(purposeLocationId)
        //   - 关联合同单号(contractBusNo)
        for(WkfSupplyRequest row : rows){
            //创建wkf_supply_delivery
            WkfSupplyDelivery delivery = new WkfSupplyDelivery();
            //生成UUID
            delivery.setId(UUID.randomUUID().toString().replace("-",""));
            //关联出库单
            delivery.setRequestId(row.getBusNo());
            //类型和状态
            delivery.setTypeEnum("WAREHOUSE_STOCKOUT");
            delivery.setStatusEnum("DISPENSED");
            //物品信息
            delivery.setItemTable(row.getItemTable());
            delivery.setItemId(row.getItemId());
            delivery.setUnitCode(row.getUnitCode());
            delivery.setQuantity(row.getItemQuantity());
            delivery.setLotNumber(row.getLotNumber());
            delivery.setTraceNo(row.getTraceNo());
            delivery.setSupplierId(row.getSupplierId());
            delivery.setPractitionerId(row.getPractitionerId());
            delivery.setOccurrenceTime(now);
            // 关联合同（可选）
            if (StringUtils.hasText(contractBusNo)) {
                delivery.setBasedOnTable("contract");
                delivery.setBasedOnIds(contractBusNo);
            }
            // 审计字段
            delivery.setCreatedAt(now);
            delivery.setUpdatedAt(now);
            delivery.setCreatedBy(userId);
            delivery.setUpdatedBy(userId);
            delivery.setTenantId(row.getTenantId());
            delivery.setIsDeleted(false);
            delivery.setVersion(0);
            supplyDeliveryList.add(delivery);
        }
        //批量插入
        supplyDeliveryService.saveBatch(supplyDeliveryList);
    }

    /**
     * 源库库存扣减 + 目标药房库存增加
     */
    private void adjustInventoryForStockOut(List<WkfSupplyRequest> rows, LocalDateTime now, String userId) {
        for (WkfSupplyRequest row : rows) {
            BigDecimal quantity = row.getItemQuantity();
            if (quantity == null) {
                throw new IllegalStateException("出库数量不能为空，行ID: " + row.getId());
            }

            // 目标药房库位非空校验
            if (!StringUtils.hasText(row.getPurposeLocationId())) {
                throw new IllegalStateException("目标药房库位不能为空，行ID: " + row.getId());
            }
            // 源库库位非空校验
            if (!StringUtils.hasText(row.getSourceLocationId())) {
                throw new IllegalStateException("源库库位不能为空，行ID: " + row.getId());
            }

            // 目标药房库存增加（purposeLocationId = 目标药房库位）
            // 先尝试 UPDATE 已有批次，匹配不到则 INSERT 新行
            UpdateWrapper<WkfInventoryItem> addWrapper = new UpdateWrapper<WkfInventoryItem>()
                    .eq("item_table", row.getItemTable())
                    .eq("item_id", row.getItemId())
                    .eq("location_id", row.getPurposeLocationId())
                    .eq("lot_number", row.getLotNumber())
                    .eq("unit_code", row.getUnitCode())
                    .eq("is_deleted", 0)
                    .setSql("quantity = quantity + " + quantity.toPlainString())
                    .set("updated_at", now)
                    .set("updated_by", userId);
            int affected = inventoryItemMapper.update(null, addWrapper);
            if (affected <= 0) {
                // 目标药房无此批次，插入新库存行
                // supplier_id 从源库库存行带入
                String supplierId = getInventorySupplier(row.getItemTable(), row.getItemId(),
                        row.getSourceLocationId(), row.getLotNumber(), row.getUnitCode());

                WkfInventoryItem newItem = new WkfInventoryItem();
                newItem.setId(UUID.randomUUID().toString().replace("-", ""));
                newItem.setTenantId(row.getTenantId());
                newItem.setItemTable(row.getItemTable());
                newItem.setItemId(row.getItemId());
                newItem.setName(row.getItemTable() + ":" + row.getItemId());
                newItem.setUnitCode(row.getUnitCode());
                newItem.setQuantity(quantity);
                newItem.setLotNumber(row.getLotNumber());
                newItem.setLocationId(row.getPurposeLocationId());
                newItem.setSupplierId(supplierId);
                newItem.setInventoryStatusEnum("ACTIVE");
                newItem.setProductionDate(row.getStartTime());
                newItem.setExpirationDate(row.getEndTime());
                newItem.setTraceNo(row.getTraceNo());
                newItem.setCreatedAt(now);
                newItem.setCreatedBy(userId);
                newItem.setUpdatedAt(now);
                newItem.setUpdatedBy(userId);
                newItem.setIsDeleted(false);
                newItem.setVersion(0);
                inventoryItemMapper.insert(newItem);
            }

            // 源库库存充足性校验
            BigDecimal currentQty = getInventoryQuantity(row.getItemTable(), row.getItemId(),
                    row.getSourceLocationId(), row.getLotNumber(), row.getUnitCode());
            if (currentQty == null) {
                throw new IllegalStateException("源库库存记录不存在，locationId: "
                        + row.getSourceLocationId() + ", itemId: " + row.getItemId());
            }
            if (currentQty.compareTo(quantity) < 0) {
                throw new IllegalStateException(String.format(
                        "源库库存不足，locationId: %s, itemId: %s, 当前库存: %s, 需出库: %s",
                        row.getSourceLocationId(), row.getItemId(), currentQty.toPlainString(), quantity.toPlainString()));
            }

            // 源库库存扣减
            UpdateWrapper<WkfInventoryItem> deductWrapper = new UpdateWrapper<WkfInventoryItem>()
                    .eq("item_table", row.getItemTable())
                    .eq("item_id", row.getItemId())
                    .eq("location_id", row.getSourceLocationId())
                    .eq("lot_number", row.getLotNumber())
                    .eq("unit_code", row.getUnitCode())
                    .eq("is_deleted", 0)
                    .setSql("quantity = quantity - " + quantity.toPlainString())
                    .set("updated_at", now)
                    .set("updated_by", userId);
            inventoryItemMapper.update(null, deductWrapper);
        }
    }

    /**
     * 查询指定库存项的 supplier_id（用于目标药房 INSERT 时带入）
     */
    private String getInventorySupplier(String itemTable, String itemId, String locationId,
                                         String lotNumber, String unitCode) {
        WkfInventoryItem item = inventoryItemMapper.selectOne(
                new QueryWrapper<WkfInventoryItem>()
                        .eq("item_table", itemTable)
                        .eq("item_id", itemId)
                        .eq("location_id", locationId)
                        .eq("lot_number", lotNumber)
                        .eq("unit_code", unitCode)
                        .eq("is_deleted", 0)
                        .last("LIMIT 1"));
        return (item != null && StringUtils.hasText(item.getSupplierId()))
                ? item.getSupplierId() : "";
    }

    /**
     * 查询指定库存项的当前数量
     */
    private BigDecimal getInventoryQuantity(String itemTable, String itemId, String locationId,
                                            String lotNumber, String unitCode) {
        WkfInventoryItem item = inventoryItemMapper.selectOne(
                new QueryWrapper<WkfInventoryItem>()
                        .eq("item_table", itemTable)
                        .eq("item_id", itemId)
                        .eq("location_id", locationId)
                        .eq("lot_number", lotNumber)
                        .eq("unit_code", unitCode)
                        .eq("is_deleted", 0)
                        .last("LIMIT 1"));
        return item != null ? item.getQuantity() : null;
    }


    @Resource
    private IWkfSupplyRequestService supplyRequestService;
    /**
     * 生成药房入库申请单（wkf_supply_request，type_enum = WAREHOUSE_STOCKIN）
     */
    private void createPharmacyInboundRequest(List<WkfSupplyRequest> rows, String contractBusNo, LocalDateTime now, String userId) {
        // 生成入库单号（整批共用一个 busNo，优先从合同单号派生）
        String inboundBusNo = generateInboundBusNo(rows.get(0).getBusNo(), contractBusNo);
        List<WkfSupplyRequest> list = new ArrayList<>();
        // 遍历 rows，向 wkf_supply_request 插入入库记录：
        //   - type_enum = "WAREHOUSE_STOCKIN"
        //   - status_enum = "DRAFT"
        //   - purposeLocationId 不变（目标药房）
        //   - busNo 使用新的入库单号（从 contractBusNo 派生或自动生成）
        for (WkfSupplyRequest row : rows) {
            WkfSupplyRequest inbound = new WkfSupplyRequest();
            // 主键
            inbound.setId(UUID.randomUUID().toString().replace("-", ""));
            // 单据信息
            inbound.setBusNo(inboundBusNo);
            inbound.setTypeEnum("WAREHOUSE_STOCKIN");
            inbound.setStatusEnum("PENDING_APPROVAL");
            inbound.setOriginalBusNo(row.getBusNo());  // 关联源出库单
            inbound.setCategoryEnum(row.getCategoryEnum());
            inbound.setIntentEnum(row.getIntentEnum());
            inbound.setPriorityEnum(row.getPriorityEnum());
            // 库位：保持方向一致（源药库→目标药房）
            inbound.setSourceLocationId(row.getSourceLocationId());
            inbound.setSourceTypeEnum(row.getSourceTypeEnum());
            inbound.setPurposeLocationId(row.getPurposeLocationId());
            inbound.setPurposeTypeEnum(row.getPurposeTypeEnum());
            // 物品信息（从出库单行复制）
            inbound.setItemTable(row.getItemTable());
            inbound.setItemId(row.getItemId());
            inbound.setItemQuantity(row.getItemQuantity());
            inbound.setUnitCode(row.getUnitCode());
            inbound.setLotNumber(row.getLotNumber());
            inbound.setTraceNo(row.getTraceNo());
            inbound.setPrice(row.getPrice());
            inbound.setTotalPrice(row.getTotalPrice());
            inbound.setSupplierId(row.getSupplierId());
            inbound.setPractitionerId(row.getPractitionerId());
            inbound.setApplicantId(userId);
            inbound.setOccurrenceTime(now);
            // 审计字段
            inbound.setCreatedAt(now);
            inbound.setUpdatedAt(now);
            inbound.setCreatedBy(userId);
            inbound.setUpdatedBy(userId);
            inbound.setTenantId(row.getTenantId());
            inbound.setIsDeleted(false);
            inbound.setVersion(0);
            list.add(inbound);
        }
        supplyRequestService.saveBatch(list);
    }

    /**
     * 生成入库单号
     * <p>派生优先级：contractBusNo → stockOutBusNo → 时间戳fallback</p>
     * <p>格式: CSI + 数字部分（CSI = Clinic Stock In，区别于出库 CSO = Clinic Stock Out）</p>
     *
     * @param stockOutBusNo 源出库单号（如 CSO2026-07-270001）
     * @param contractBusNo 关联合同单号（可选，如 CT202607180001）
     */
    private String generateInboundBusNo(String stockOutBusNo, String contractBusNo) {
        String base = StringUtils.hasText(contractBusNo) ? contractBusNo : stockOutBusNo;
        // 提取数字部分: "CT202607180001" → "202607180001", "CSO2026-07-27-0001" → "202607270001"
        String numericPart = base.replaceAll("[^0-9]", "");
        if (numericPart.length() >= 8) {
            return "CSI" + numericPart;
        }
        // Fallback: 当天日期 + 序号
        DateTimeFormatter fmt = DateTimeFormatter.ofPattern("yyyyMMdd");
        return "CSI" + LocalDate.now().format(fmt) + "0001";
    }

    /**
     * 解析审批时间：优先 DTO 传入值，否则当前时间
     */
    private LocalDateTime resolveApprovalClock(String approvalClock) {
        if (StringUtils.hasText(approvalClock)) {
            try {
                return LocalDateTime.parse(approvalClock.replace("T", "T").substring(0, 19));
            } catch (Exception ignored) {}
        }
        return LocalDateTime.now();
    }

    @FunctionalInterface
    private interface StatusUpdateCustomizer {
        void customize(UpdateWrapper<WkfSupplyRequest> wrapper);
    }
}
