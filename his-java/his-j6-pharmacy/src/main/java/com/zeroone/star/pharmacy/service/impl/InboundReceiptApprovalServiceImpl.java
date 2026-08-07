package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.MedMedicationDefinition;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IInboundReceiptApprovalService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.AgreeInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.RejectInboundReceiptDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.SubmitInboundReceiptApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.WithdrawInboundReceiptApprovalDTO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundApprovalResultVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * <p>进货单审批业务实现</p>
 *
 * @author AB
 * @since 2026-07-27
 */
@Service
public class InboundReceiptApprovalServiceImpl implements IInboundReceiptApprovalService {

    /** 进货单类型编码，与前端 WHS_IN_SUPPLY_TYPE_CODE 一致 */
    private static final String TYPE_INBOUND_RECEIPT = "CABINET_INBOUND_RECEIPT";

    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    private static final String STATUS_AGREE = "AGREE";
    private static final String STATUS_REJECT = "REJECT";

    @Resource
    private WkfSupplyRequestMapper supplyRequestMapper;

    @Resource
    private WkfInventoryItemMapper inventoryItemMapper;

    @Resource
    private MedMedicationDefinitionMapper medicationDefinitionMapper;

    @Resource
    private UserHolder userHolder;

    // ==================== 1. 提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalResultVO submit(SubmitInboundReceiptApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireInboundReceipt(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_DRAFT, STATUS_WITHDRAW, STATUS_REJECT);

        String approverId = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : null;
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();

        int affected = supplyRequestMapper.updateInboundToPending(
                dto.getBusNo(), TYPE_INBOUND_RECEIPT, rows.get(0).getStatusEnum(),
                approverId, userId, now, now, userId);
        requireUpdated(affected);

        return new InboundApprovalResultVO(true, STATUS_PENDING_APPROVAL);
    }

    // ==================== 2. 撤回审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalResultVO withdraw(WithdrawInboundReceiptApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireInboundReceipt(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();

        int affected = supplyRequestMapper.updateInboundToWithdraw(
                dto.getBusNo(), TYPE_INBOUND_RECEIPT, rows.get(0).getStatusEnum(), now, userId);
        requireUpdated(affected);

        return new InboundApprovalResultVO(true, STATUS_WITHDRAW);
    }

    // ==================== 3. 审批通过 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalResultVO agree(AgreeInboundReceiptDTO dto) {
        List<WkfSupplyRequest> rows = requireInboundReceipt(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        LocalDateTime approvalTime = resolveApprovalClock(dto.getApprovalClock());
        String userId = currentUserId();
        String approverId = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : userId;

        // 审批通过后执行入库：逐行写入/更新库存
        inboundInventory(rows, now, userId);

        int affected = supplyRequestMapper.updateInboundToAgree(
                dto.getBusNo(), TYPE_INBOUND_RECEIPT, rows.get(0).getStatusEnum(),
                approverId, approvalTime, now, userId);
        requireUpdated(affected);

        return new InboundApprovalResultVO(true, STATUS_AGREE);
    }

    // ==================== 4. 驳回审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundApprovalResultVO reject(RejectInboundReceiptDTO dto) {
        if (!StringUtils.hasText(dto.getRejectReason())) {
            throw new IllegalArgumentException("驳回原因不能为空");
        }

        List<WkfSupplyRequest> rows = requireInboundReceipt(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        String approverId = StringUtils.hasText(dto.getApproverId()) ? dto.getApproverId() : userId;

        int affected = supplyRequestMapper.updateInboundToReject(
                dto.getBusNo(), TYPE_INBOUND_RECEIPT, rows.get(0).getStatusEnum(),
                approverId, now, dto.getRejectReason(), now, userId);
        requireUpdated(affected);

        return new InboundApprovalResultVO(true, STATUS_REJECT);
    }

    // ==================== 入库辅助 ====================

    /**
     * 审批通过后，将进货单明细逐行写入库存。
     * 若库存中已存在相同 item_table + item_id + location_id + lot_number + unit_code 的行则累加 quantity，
     * 否则插入新行。
     */
    private void inboundInventory(List<WkfSupplyRequest> rows, LocalDateTime now, String userId) {
        // 批量预取药品名称，避免循环内逐行查库（N+1）
        Map<String, MedMedicationDefinition> medMap = preloadMedicationMap(rows);

        for (WkfSupplyRequest row : rows) {
            if (row.getItemQuantity() == null || row.getItemQuantity().compareTo(BigDecimal.ZERO) <= 0) {
                throw new IllegalStateException("进货数量必须大于零，行ID: " + row.getId());
            }

            // 尝试更新已有库存行（按五元组匹配）
            int updated = inventoryItemMapper.addQuantity(
                    row.getItemTable(),
                    row.getItemId(),
                    row.getPurposeLocationId(),
                    row.getLotNumber(),
                    row.getUnitCode(),
                    row.getItemQuantity(),
                    row.getSupplierId(),
                    row.getEndTime(),
                    now,
                    userId);

            // 无匹配行则插入新库存行
            if (updated <= 0) {
                WkfInventoryItem item = new WkfInventoryItem();
                item.setId(UUID.randomUUID().toString().replace("-", ""));
                item.setItemTable(row.getItemTable());
                item.setItemId(row.getItemId());
                item.setName(resolveItemName(row.getItemTable(), row.getItemId(), medMap));
                item.setUnitCode(row.getUnitCode());
                item.setQuantity(row.getItemQuantity());
                item.setLotNumber(row.getLotNumber());
                item.setLocationId(row.getPurposeLocationId());
                item.setSupplierId(row.getSupplierId());
                item.setInventoryStatusEnum("ACTIVE");
                item.setProductionDate(row.getStartTime());
                item.setExpirationDate(row.getEndTime());
                item.setTraceNo(row.getTraceNo());
                item.setCreatedAt(now);
                item.setCreatedBy(userId);
                item.setUpdatedAt(now);
                item.setUpdatedBy(userId);
                item.setIsDeleted(false);
                item.setVersion(0);
                inventoryItemMapper.insert(item);
            }
        }
    }

    /**
     * 批量预取药品定义，按 itemId → entity 放入 Map，供 resolveItemName 循环内取用。
     */
    private Map<String, MedMedicationDefinition> preloadMedicationMap(List<WkfSupplyRequest> rows) {
        List<String> medIds = rows.stream()
                .filter(r -> "med_medication_definition".equals(r.getItemTable()))
                .map(WkfSupplyRequest::getItemId)
                .filter(StringUtils::hasText)
                .distinct()
                .collect(Collectors.toList());
        if (medIds.isEmpty()) {
            return Map.of();
        }
        return medicationDefinitionMapper.selectBatchIds(medIds).stream()
                .collect(Collectors.toMap(MedMedicationDefinition::getId, Function.identity()));
    }

    /**
     * 根据物品定义表名和ID查询物品名称。
     * 当前支持 med_medication_definition，其他表返回 itemTable:itemId 占位。
     */
    private String resolveItemName(String itemTable, String itemId, Map<String, MedMedicationDefinition> medMap) {
        if ("med_medication_definition".equals(itemTable)) {
            MedMedicationDefinition med = medMap.get(itemId);
            if (med != null && StringUtils.hasText(med.getName())) {
                return med.getName();
            }
        }
        return itemTable + ":" + itemId;
    }

    // ==================== 校验辅助 ====================

    private List<WkfSupplyRequest> requireInboundReceipt(String busNo) {
        List<WkfSupplyRequest> rows = supplyRequestMapper.selectInboundRowsForApproval(
                busNo, TYPE_INBOUND_RECEIPT);
        if (rows.isEmpty()) {
            throw new IllegalArgumentException("进货单不存在: " + busNo);
        }
        return rows;
    }

    private void requireSameStatus(List<WkfSupplyRequest> rows) {
        Set<String> statuses = rows.stream()
                .map(WkfSupplyRequest::getStatusEnum)
                .collect(Collectors.toSet());
        if (statuses.size() != 1) {
            throw new IllegalStateException("进货单内各行状态不一致: " + statuses);
        }
    }

    private void requireAnyStatus(String currentStatus, String... allowed) {
        for (String s : allowed) {
            if (s.equals(currentStatus)) {
                return;
            }
        }
        throw new IllegalStateException("当前状态 " + currentStatus + " 不允许执行此操作");
    }

    private void requireUpdated(int affectedRows) {
        if (affectedRows <= 0) {
            throw new IllegalStateException("更新审批状态失败，可能已被并发修改");
        }
    }

    // ==================== 用户辅助 ====================

    private LocalDateTime resolveApprovalClock(String approvalClock) {
        if (StringUtils.hasText(approvalClock)) {
            try {
                return LocalDateTime.parse(approvalClock.replace("T", "T").substring(0, 19));
            } catch (Exception ignored) {}
        }
        return LocalDateTime.now();
    }

    private String currentUserId() {
        try {
            UserDTO user = userHolder.getCurrentUser();
            if (user != null && StringUtils.hasText(user.getId())) {
                return user.getId();
            }
        } catch (Exception ignored) {
            // 无网关 header 时容错
        }
        return "system";
    }
}
