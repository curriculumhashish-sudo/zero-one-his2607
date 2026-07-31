package com.zeroone.star.pharmacy.service.impl;

import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeWithdrawApprovalDTO;
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
    private static final String TYPE_WAREHOUSE_STOCKTAKE = "PHARMACY_STOCKTAKE";
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
    public Boolean submit(StocktakeSubmitApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_DRAFT, STATUS_WITHDRAW, STATUS_REJECT);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        int affectedRows = supplyRequestMapper.updateStocktakeToPending(
                dto.getBusNo(), TYPE_WAREHOUSE_STOCKTAKE, rows.get(0).getStatusEnum(), userId,
                dto.getApproverId(), now, now, userId);
        requireUpdated(affectedRows);
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean withdraw(StocktakeWithdrawApprovalDTO dto) {
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        int affectedRows = supplyRequestMapper.updateStocktakeToWithdraw(
                dto.getBusNo(), TYPE_WAREHOUSE_STOCKTAKE, rows.get(0).getStatusEnum(), now, userId);
        requireUpdated(affectedRows);
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean agree(StocktakeAgreeApprovalDTO dto) {
        requireAgreeStatus(dto.getStatus());
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        adjustInventory(rows, now, userId);
        int affectedRows = supplyRequestMapper.updateStocktakeToAgree(
                dto.getBusNo(), TYPE_WAREHOUSE_STOCKTAKE, rows.get(0).getStatusEnum(), userId, now, now, userId);
        requireUpdated(affectedRows);
        return true;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public Boolean reject(StocktakeRejectApprovalDTO dto) {
        if (!StringUtils.hasText(dto.getChangeReason())) {
            throw new IllegalArgumentException("changeReason cannot be blank when rejecting approval");
        }
        List<WkfSupplyRequest> rows = requireWarehouseStocktake(dto.getBusNo());
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL);

        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        String reason = buildReason(dto);
        int affectedRows = supplyRequestMapper.updateStocktakeToReject(
                dto.getBusNo(), TYPE_WAREHOUSE_STOCKTAKE, rows.get(0).getStatusEnum(), userId, now, reason, now, userId);
        requireUpdated(affectedRows);
        return true;
    }

    private List<WkfSupplyRequest> requireWarehouseStocktake(String busNo) {
        List<WkfSupplyRequest> rows = supplyRequestMapper.selectStocktakeRowsForApproval(busNo, TYPE_WAREHOUSE_STOCKTAKE);
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

    private void requireAgreeStatus(String status) {
        if (StringUtils.hasText(status) && !STATUS_AGREE.equals(status)) {
            throw new IllegalArgumentException("status must be AGREE when approving stocktake");
        }
    }

    private void requireUpdated(int affectedRows) {
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
            int affectedRows = inventoryItemMapper.updateQuantityForStocktake(
                    row.getItemTable(),
                    row.getItemId(),
                    row.getPurposeLocationId(),
                    row.getLotNumber(),
                    row.getUnitCode(),
                    actualQuantity,
                    now,
                    userId);
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

    private String buildReason(StocktakeRejectApprovalDTO dto) {
        if (StringUtils.hasText(dto.getChangeReasonCode())) {
            return dto.getChangeReasonCode() + ":" + dto.getChangeReason();
        }
        return dto.getChangeReason();
    }
}
