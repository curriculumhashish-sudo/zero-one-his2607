package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeApprovalDTO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

@Service
public class StocktakeApprovalServiceImpl implements IStocktakeApprovalService {
    private static final String TYPE_WAREHOUSE_STOCKTAKE = "CABINET_STOCKTAKE";
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
    private UserHolder userHolder;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean submit(StocktakeApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_DRAFT, STATUS_WITHDRAW, STATUS_REJECT);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        updateStatus(dto.getBusNo(), STATUS_PENDING_APPROVAL, wrapper -> wrapper
                .set("applicant_id", userId)
                .set("apply_time", now)
                .set("approver_id", null)
                .set("approval_time", null)
                .set("reason", null)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean withdraw(StocktakeApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        updateStatus(dto.getBusNo(), STATUS_WITHDRAW, wrapper -> wrapper
                .set("approver_id", null)
                .set("approval_time", null)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean agree(StocktakeApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        adjustInventory(rows, now, userId);
        updateStatus(dto.getBusNo(), STATUS_AGREE, wrapper -> wrapper
                .set("approver_id", userId)
                .set("approval_time", now)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean reject(StocktakeApprovalDTO dto) {
        if (!StringUtils.hasText(dto.getChangeReason())) {
            throw new IllegalArgumentException("changeReason cannot be blank when rejecting approval");
        }
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        String reason = buildReason(dto);
        updateStatus(dto.getBusNo(), STATUS_REJECT, wrapper -> wrapper
                .set("approver_id", userId)
                .set("approval_time", now)
                .set("reason", reason)
                .set("updated_at", now)
                .set("updated_by", userId));
        return true;
    }

    private List<WkfSupplyRequest> requireWarehouseStocktake(String busNo) {
        List<WkfSupplyRequest> rows = supplyRequestMapper.selectList(new QueryWrapper<WkfSupplyRequest>()
                .eq("bus_no", busNo)
                .eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE)
                .eq("is_deleted", 0));
        if (rows.isEmpty()) {
            throw new IllegalArgumentException("warehouse stocktake order does not exist: " + busNo);
        }
        return rows;
    }

    private void requireSameStatus(List<WkfSupplyRequest> rows) {
        Set<String> statuses = rows.stream()
                .map(WkfSupplyRequest::getStatusEnum)
                .collect(Collectors.toSet());
        if (statuses.size() != 1) {
            throw new IllegalStateException("stocktake order has inconsistent row statuses: " + statuses);
        }
    }

    private void requireAnyStatus(String currentStatus, String... allowedStatuses) {
        for (String allowedStatus : allowedStatuses) {
            if (allowedStatus.equals(currentStatus)) {
                return;
            }
        }
        throw new IllegalStateException("current status " + currentStatus + " does not allow this approval action");
    }

    private void updateStatus(String busNo, String status, StatusUpdateCustomizer customizer) {
        UpdateWrapper<WkfSupplyRequest> wrapper = new UpdateWrapper<WkfSupplyRequest>()
                .eq("bus_no", busNo)
                .eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE)
                .eq("is_deleted", 0)
                .set("status_enum", status);
        customizer.customize(wrapper);
        int affectedRows = supplyRequestMapper.update(null, wrapper);
        if (affectedRows <= 0) {
            throw new IllegalStateException("failed to update stocktake approval status");
        }
    }

    private void adjustInventory(List<WkfSupplyRequest> rows, LocalDateTime now, String userId) {
        for (WkfSupplyRequest row : rows) {
            BigDecimal actualQuantity = row.getTotalQuantity();
            if (actualQuantity == null) {
                throw new IllegalStateException("stocktake actual quantity cannot be null, row id: " + row.getId());
            }
            UpdateWrapper<WkfInventoryItem> wrapper = new UpdateWrapper<WkfInventoryItem>()
                    .eq("item_table", row.getItemTable())
                    .eq("item_id", row.getItemId())
                    .eq("location_id", row.getPurposeLocationId())
                    .eq("lot_number", row.getLotNumber())
                    .eq("unit_code", row.getUnitCode())
                    .eq("is_deleted", 0)
                    .set("quantity", actualQuantity)
                    .set("updated_at", now)
                    .set("updated_by", userId);
            int affectedRows = inventoryItemMapper.update(null, wrapper);
            if (affectedRows <= 0) {
                throw new IllegalStateException("inventory item not found for stocktake row id: " + row.getId());
            }
        }
    }

    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getId();
            }
        } catch (Exception ignored) {
            // Keep service usable in local tests without gateway headers.
        }
        return "system";
    }

    private String buildReason(StocktakeApprovalDTO dto) {
        if (StringUtils.hasText(dto.getChangeReasonCode())) {
            return dto.getChangeReasonCode() + ":" + dto.getChangeReason();
        }
        return dto.getChangeReason();
    }

    @FunctionalInterface
    private interface StatusUpdateCustomizer {
        void customize(UpdateWrapper<WkfSupplyRequest> wrapper);
    }
}
