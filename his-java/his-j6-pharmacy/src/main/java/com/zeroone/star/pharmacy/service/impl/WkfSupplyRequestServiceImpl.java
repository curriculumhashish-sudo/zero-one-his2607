package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;

/**
 * <p>
 * 工作流 supply request 服务实现类
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Service
public class WkfSupplyRequestServiceImpl extends ServiceImpl<WkfSupplyRequestMapper, WkfSupplyRequest> implements IWkfSupplyRequestService {
    private static final String DEFAULT_TYPE_INBOUND = "CABINET_INBOUND_RECEIPT";
    private static final String DEFAULT_CATEGORY_STOCK_SUPPLY = "STOCK_SUPPLY";
    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    private static final String STATUS_REJECT = "REJECT";
    private static final String SYSTEM_USER_ID = "system";
    private static final String DEFAULT_TENANT_ID = "1";
    private static final DateTimeFormatter BUS_NO_DATE_FORMATTER = DateTimeFormatter.ofPattern("yyyyMMdd");

    @Resource
    private UserHolder userHolder;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundReceiptSaveResultVO addInboundReceipt(InboundReceiptSaveDTO dto) {
        String busNo = StringUtils.hasText(dto.getBusNo()) ? dto.getBusNo() : generateBusNo();
        if (existsActive(busNo)) {
            throw new IllegalArgumentException("药库进货单已存在：" + busNo);
        }
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        List<WkfSupplyRequest> rows = buildRows(dto, busNo, STATUS_DRAFT, 0, now, userId, null);
        saveBatch(rows);
        return buildResult(rows.get(0), "新增成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundReceiptSaveResultVO saveInboundReceipt(InboundReceiptSaveDTO dto) {
        String busNo = requireBusNo(dto);
        List<WkfSupplyRequest> oldRows = requireEditableRows(busNo, dto.getVersion());
        int nextVersion = nextVersion(oldRows);
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        softDeleteRows(busNo, now, userId);
        List<WkfSupplyRequest> newRows = buildRows(dto, busNo, STATUS_DRAFT, nextVersion, now, userId, null);
        saveBatch(newRows);
        return buildResult(newRows.get(0), "保存成功");
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundReceiptSaveResultVO saveAndSubmitInboundReceipt(InboundReceiptSaveDTO dto) {
        String busNo = StringUtils.hasText(dto.getBusNo()) ? dto.getBusNo() : generateBusNo();
        List<WkfSupplyRequest> oldRows = listActive(busNo);
        int nextVersion = oldRows.isEmpty() ? 0 : nextVersion(requireEditableRows(busNo, dto.getVersion()));
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        if (!oldRows.isEmpty()) {
            softDeleteRows(busNo, now, userId);
        }
        List<WkfSupplyRequest> newRows = buildRows(dto, busNo, STATUS_PENDING_APPROVAL, nextVersion, now, userId, now);
        saveBatch(newRows);
        return buildResult(newRows.get(0), "保存并提交审批成功");
    }

    private List<WkfSupplyRequest> buildRows(InboundReceiptSaveDTO dto, String busNo, String status, Integer version,
                                             LocalDateTime now, String userId, LocalDateTime applyTime) {
        return dto.getLines().stream()
                .map(line -> buildRow(dto, line, busNo, status, version, now, userId, applyTime))
                .toList();
    }

    private WkfSupplyRequest buildRow(InboundReceiptSaveDTO dto, InboundReceiptSaveDTO.InboundReceiptLineDTO line,
                                      String busNo, String status, Integer version, LocalDateTime now,
                                      String userId, LocalDateTime applyTime) {
        WkfSupplyRequest row = new WkfSupplyRequest();
        row.setId(IdWorker.getIdStr());
        row.setTenantId(DEFAULT_TENANT_ID);
        row.setCreatedAt(now);
        row.setUpdatedAt(now);
        row.setCreatedBy(userId);
        row.setUpdatedBy(userId);
        row.setIsDeleted(false);
        row.setVersion(version);
        row.setBusNo(busNo);
        row.setOriginalBusNo(dto.getOriginalBusNo());
        row.setTypeEnum(valueOrDefault(dto.getTypeEnum(), DEFAULT_TYPE_INBOUND));
        row.setStatusEnum(status);
        row.setCategoryEnum(valueOrDefault(dto.getCategoryEnum(), DEFAULT_CATEGORY_STOCK_SUPPLY));
        row.setItemTable(line.getItemTable());
        row.setItemQuantity(line.getItemQuantity());
        row.setItemId(line.getItemId());
        row.setUnitCode(line.getUnitCode());
        row.setOccurrenceTime(line.getOccurrenceTime() == null ? now : line.getOccurrenceTime());
        row.setPractitionerId(dto.getPractitionerId());
        row.setSupplierId(dto.getSupplierId());
        row.setReason(dto.getReason());
        row.setSourceTypeEnum(dto.getSourceTypeEnum());
        row.setSourceLocationId(dto.getSourceLocationId());
        row.setPurposeTypeEnum(dto.getPurposeTypeEnum());
        row.setPurposeLocationId(dto.getPurposeLocationId());
        row.setApplicantId(userId);
        row.setApplyTime(applyTime);
        row.setStartTime(line.getStartTime());
        row.setEndTime(line.getEndTime());
        row.setLotNumber(line.getLotNumber());
        row.setTraceNo(line.getTraceNo());
        row.setPrice(line.getPrice());
        row.setTotalPrice(line.getTotalPrice());
        row.setRemake(line.getRemake());
        return row;
    }

    private String requireBusNo(InboundReceiptSaveDTO dto) {
        if (!StringUtils.hasText(dto.getBusNo())) {
            throw new IllegalArgumentException("保存药库进货单时业务单号不能为空");
        }
        return dto.getBusNo();
    }

    private List<WkfSupplyRequest> requireEditableRows(String busNo, Integer expectedVersion) {
        List<WkfSupplyRequest> rows = listActive(busNo);
        if (rows.isEmpty()) {
            throw new IllegalArgumentException("药库进货单不存在：" + busNo);
        }
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_DRAFT, STATUS_WITHDRAW, STATUS_REJECT);
        if (expectedVersion != null) {
            Integer currentVersion = rows.get(0).getVersion();
            if (!Objects.equals(currentVersion, expectedVersion)) {
                throw new IllegalStateException("单据版本已变化，请刷新后重试");
            }
        }
        return rows;
    }

    private List<WkfSupplyRequest> listActive(String busNo) {
        return baseMapper.selectActiveRowsByBusNo(busNo);
    }

    private boolean existsActive(String busNo) {
        return baseMapper.countActiveRowsByBusNo(busNo) > 0;
    }

    private void softDeleteRows(String busNo, LocalDateTime now, String userId) {
        int affectedRows = baseMapper.updateActiveRowsDeletedByBusNo(busNo, now, userId, now, userId);
        if (affectedRows <= 0) {
            throw new IllegalStateException("保存药库进货单失败，旧明细未更新");
        }
    }

    private void requireSameStatus(List<WkfSupplyRequest> rows) {
        long statusCount = rows.stream()
                .map(WkfSupplyRequest::getStatusEnum)
                .distinct()
                .count();
        if (statusCount != 1) {
            throw new IllegalStateException("单据明细状态不一致，不能保存");
        }
    }

    private void requireAnyStatus(String currentStatus, String... allowedStatuses) {
        for (String allowedStatus : allowedStatuses) {
            if (allowedStatus.equals(currentStatus)) {
                return;
            }
        }
        throw new IllegalStateException("当前状态不允许保存：" + currentStatus);
    }

    private int nextVersion(List<WkfSupplyRequest> rows) {
        return rows.stream()
                .map(WkfSupplyRequest::getVersion)
                .filter(Objects::nonNull)
                .max(Comparator.naturalOrder())
                .orElse(0) + 1;
    }

    private String generateBusNo() {
        return "CIR" + LocalDateTime.now().format(BUS_NO_DATE_FORMATTER) + IdWorker.getIdStr().substring(10);
    }

    private String valueOrDefault(String value, String defaultValue) {
        return StringUtils.hasText(value) ? value : defaultValue;
    }

    private InboundReceiptSaveResultVO buildResult(WkfSupplyRequest row, String actionResult) {
        InboundReceiptSaveResultVO result = new InboundReceiptSaveResultVO();
        result.setId(row.getId());
        result.setBusNo(row.getBusNo());
        result.setStatusEnum(row.getStatusEnum());
        result.setVersion(row.getVersion());
        result.setActionResult(actionResult);
        result.setApplyTime(row.getApplyTime());
        return result;
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
        return SYSTEM_USER_ID;
    }

}
