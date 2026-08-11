package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.toolkit.IdWorker;
import com.zeroone.star.pharmacy.entity.AdmDeviceDefinition;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.*;
import com.zeroone.star.pharmacy.mapper.*;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptAddDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveDTO;
import com.zeroone.star.project.dto.j6.pharmacy.inbound.InboundReceiptSaveRequestDTO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailLineVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.inbound.InboundReceiptSaveResultVO;
import jakarta.annotation.Resource;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.j6.pharmacy.*;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.PharmacyInventoryVO;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Comparator;
import java.util.List;
import java.util.Objects;
import org.springframework.util.CollectionUtils;

import java.math.BigDecimal;
import java.util.*;
import java.util.stream.Collectors;

/**
 * <p>
 * 工作流 supply request 服务实现类
 * </p>
 *
 * @author leisurely
 * @since 2026-07-24
 */
@Service
@Slf4j
public class WkfSupplyRequestServiceImpl extends ServiceImpl<WkfSupplyRequestMapper, WkfSupplyRequest> implements IWkfSupplyRequestService {
    private static final String DEFAULT_TYPE_INBOUND = "CABINET_INBOUND_RECEIPT";
    private static final String DEFAULT_CATEGORY_STOCK_SUPPLY = "STOCK_SUPPLY";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_APPROVAL = "APPROVAL";
    private static final String STATUS_WITHDRAW = "WITHDRAW";
    private static final String STATUS_REJECT = "REJECT";
    private static final String SYSTEM_USER_ID = "system";
    private static final String DEFAULT_TENANT_ID = "1";
    private static final DateTimeFormatter BUS_NO_DATE_FORMATTER = DateTimeFormatter.ofPattern("yyyyMMdd");

    @Resource
    private UserHolder userHolder;

    private static final String DEFAULT_SOURCE_TYPE = "SUPPLIER";
    private static final String DEFAULT_PURPOSE_TYPE = "WAREHOUSE";
    private static final String DEFAULT_CATEGORY = "PURCHASE";

    @Resource
    private CusSupplierMapper cusSupplierMapper;

    @Resource
    private AdmDeviceDefinitionMapper admDeviceDefinitionMapper;

    // ==================== 1. 新增空白草稿 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundReceiptDetailVO addInboundReceipt(InboundReceiptAddDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("新增药库进货单入参不能为空");
        }
        if (!StringUtils.hasText(dto.getPurposeLocationId())) {
            throw new IllegalArgumentException("目的库房不能为空");
        }
        if (!StringUtils.hasText(dto.getSupplierId())) {
            throw new IllegalArgumentException("供应商不能为空");
        }

        String busNo = generateBusNo();
        if (existsActive(busNo)) {
            throw new IllegalArgumentException("药库进货单已存在：" + busNo);
        }
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        String practitionerId = StringUtils.hasText(dto.getPractitionerId())
                ? dto.getPractitionerId() : userId;

        // 插入一条仅含头信息的锚点行（无明细行数据）
        WkfSupplyRequest anchor = buildAnchorRow(busNo, dto, practitionerId, now, userId);
        save(anchor);

        return buildEnrichedDetailVO(anchor, List.of(), now, userId);
    }

    // ==================== 2. 保存草稿 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundReceiptDetailVO saveInboundReceipt(InboundReceiptSaveRequestDTO dto) {
        validateSaveRequest(dto);
        String busNo = requireBusNoFromSaveRequest(dto);
        List<WkfSupplyRequest> oldRows = requireEditableRows(busNo, null);
        int nextVersion = nextVersion(oldRows);
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        softDeleteRows(busNo, now, userId);
        List<WkfSupplyRequest> newRows = buildRowsFromSaveRequest(dto, busNo, STATUS_PENDING_APPROVAL, nextVersion, now, userId, null);
        saveBatch(newRows);
        return buildEnrichedDetailVO(newRows.get(0), newRows, now, userId);
    }

    // ==================== 3. 保存并提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public InboundReceiptDetailVO saveAndSubmitInboundReceipt(InboundReceiptSaveRequestDTO dto) {
        validateSaveRequest(dto);
        String busNo = StringUtils.hasText(dto.getBusNo()) ? dto.getBusNo() : generateBusNo();
        List<WkfSupplyRequest> oldRows = listActive(busNo);
        int nextVersion = oldRows.isEmpty() ? 0 : nextVersion(requireEditableRows(busNo, null));
        LocalDateTime now = LocalDateTime.now();
        String userId = currentUserId();
        if (!oldRows.isEmpty()) {
            softDeleteRows(busNo, now, userId);
        }
        List<WkfSupplyRequest> newRows = buildRowsFromSaveRequest(dto, busNo, STATUS_APPROVAL, nextVersion, now, userId, now);
        saveBatch(newRows);
        return buildEnrichedDetailVO(newRows.get(0), newRows, now, userId);
    }

    // ==================== 校验 ====================

    private void validateSaveRequest(InboundReceiptSaveRequestDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("药库进货单入参不能为空");
        }
        if (dto.getLines() == null || dto.getLines().isEmpty()) {
            throw new IllegalArgumentException("药库进货单明细不能为空");
        }
        for (int i = 0; i < dto.getLines().size(); i++) {
            InboundReceiptSaveRequestDTO.InboundReceiptLineDTO line = dto.getLines().get(i);
            if (line == null) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行明细不能为空");
            }
            if (!StringUtils.hasText(line.getItemTable())) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行项目表不能为空");
            }
            if (!StringUtils.hasText(line.getItemId())) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行项目标识不能为空");
            }
            if (!StringUtils.hasText(line.getUnitCode())) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行单位编码不能为空");
            }
            if (line.getItemQuantity() == null || line.getItemQuantity().compareTo(BigDecimal.ZERO) <= 0) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行数量必须大于0");
            }
        }
    }

    // ==================== 构建锚点行（新增时） ====================

    private WkfSupplyRequest buildAnchorRow(String busNo, InboundReceiptAddDTO dto,
                                            String practitionerId, LocalDateTime now, String userId) {
        WkfSupplyRequest row = new WkfSupplyRequest();
        row.setId(IdWorker.getIdStr());
        row.setTenantId(DEFAULT_TENANT_ID);
        row.setCreatedAt(now);
        row.setUpdatedAt(now);
        row.setCreatedBy(userId);
        row.setUpdatedBy(userId);
        row.setIsDeleted(false);
        row.setVersion(0);
        row.setBusNo(busNo);
        row.setOriginalBusNo(dto.getOriginalBusNo());
        row.setTypeEnum(DEFAULT_TYPE_INBOUND);
        row.setStatusEnum(STATUS_PENDING_APPROVAL);
        row.setCategoryEnum(DEFAULT_CATEGORY);
        row.setPractitionerId(practitionerId);
        row.setSupplierId(dto.getSupplierId());
        row.setSourceTypeEnum(DEFAULT_SOURCE_TYPE);
        row.setPurposeTypeEnum(DEFAULT_PURPOSE_TYPE);
        row.setPurposeLocationId(dto.getPurposeLocationId());
        row.setReason(dto.getReason());
        row.setApplicantId(userId);
        // 锚点行无实际明细，填入 NOT NULL 字段的空默认值
        row.setItemTable("");
        row.setItemId("");
        row.setItemQuantity(BigDecimal.ZERO);
        row.setUnitCode("");
        return row;
    }

    // ==================== 构建保存行 ====================

    private List<WkfSupplyRequest> buildRowsFromSaveRequest(InboundReceiptSaveRequestDTO dto, String busNo,
                                                            String status, Integer version,
                                                            LocalDateTime now, String userId,
                                                            LocalDateTime applyTime) {
        return dto.getLines().stream()
                .map(line -> buildRowFromSaveLine(dto, line, busNo, status, version, now, userId, applyTime))
                .toList();
    }

    private WkfSupplyRequest buildRowFromSaveLine(InboundReceiptSaveRequestDTO dto,
                                                  InboundReceiptSaveRequestDTO.InboundReceiptLineDTO line,
                                                  String busNo, String status, Integer version,
                                                  LocalDateTime now, String userId, LocalDateTime applyTime) {
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
        row.setCategoryEnum(valueOrDefault(dto.getCategoryEnum(), DEFAULT_CATEGORY));
        row.setItemTable(line.getItemTable());
        row.setItemQuantity(line.getItemQuantity());
        row.setItemId(line.getItemId());
        row.setUnitCode(line.getUnitCode());
        row.setOccurrenceTime(line.getOccurrenceTime() == null ? now : line.getOccurrenceTime());
        row.setPractitionerId(dto.getPractitionerId());
        row.setSupplierId(dto.getSupplierId());
        row.setReason(dto.getReason());
        row.setSourceTypeEnum(valueOrDefault(dto.getSourceTypeEnum(), DEFAULT_SOURCE_TYPE));
        row.setSourceLocationId(dto.getSourceLocationId());
        row.setPurposeTypeEnum(valueOrDefault(dto.getPurposeTypeEnum(), DEFAULT_PURPOSE_TYPE));
        row.setPurposeLocationId(dto.getPurposeLocationId());
        row.setApplicantId(StringUtils.hasText(dto.getApplicantId()) ? dto.getApplicantId() : userId);
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

    // ==================== 构建返回VO ====================

    /**
     * 组装完整的文档详情视图（含头信息名称解析 + 明细行）。
     */
    private InboundReceiptDetailVO buildEnrichedDetailVO(WkfSupplyRequest header,
                                                         List<WkfSupplyRequest> rows,
                                                         LocalDateTime now, String userId) {
        InboundReceiptDetailVO vo = new InboundReceiptDetailVO();
        vo.setBusNo(header.getBusNo());
        vo.setOriginalBusNo(header.getOriginalBusNo());
        vo.setTypeEnum(header.getTypeEnum());
        vo.setStatusEnum(header.getStatusEnum());
        vo.setSupplierId(header.getSupplierId());
        vo.setSourceLocationId(header.getSourceLocationId());
        vo.setPurposeLocationId(header.getPurposeLocationId());
        vo.setSourceTypeEnum(header.getSourceTypeEnum());
        vo.setPurposeTypeEnum(header.getPurposeTypeEnum());
        vo.setCategoryEnum(header.getCategoryEnum());
        vo.setRowVersion(header.getVersion());
        vo.setApplyTime(header.getApplyTime());
        vo.setApplicantId(header.getApplicantId());
        vo.setApprovalTime(header.getApprovalTime());
        vo.setReason(header.getReason());
        vo.setPractitionerId(header.getPractitionerId());
        vo.setCreatorId(header.getCreatedBy());
        vo.setApproverId(header.getApproverId());

        // ---- 解析名称 ----
        // 供应商名称
        if (StringUtils.hasText(header.getSupplierId())) {
            try {
                com.zeroone.star.pharmacy.entity.CusSupplier supplier =
                        cusSupplierMapper.selectById(header.getSupplierId());
                if (supplier != null) {
                    vo.setSupplierName(supplier.getName());
                }
            } catch (Exception ignored) { /* ignore */ }
        }
        // 库房名称
        if (StringUtils.hasText(header.getPurposeLocationId())) {
            try {
                AdmLocation location = admLocationMapper.selectById(header.getPurposeLocationId());
                if (location != null) {
                    vo.setPurposeLocationName(location.getName());
                }
            } catch (Exception ignored) { /* ignore */ }
        }
        // 制单人名称
        if (StringUtils.hasText(header.getCreatedBy())) {
            try {
                AdmPractitioner creator = admPractitionerMapper.selectById(header.getCreatedBy());
                if (creator != null) {
                    vo.setCreatorName(creator.getName());
                }
            } catch (Exception ignored) { /* ignore */ }
        }
        // 申请人名称
        if (StringUtils.hasText(header.getApplicantId())) {
            try {
                AdmPractitioner applicant = admPractitionerMapper.selectById(header.getApplicantId());
                if (applicant != null) {
                    vo.setApplicantName(applicant.getName());
                }
            } catch (Exception ignored) { /* ignore */ }
        }
        // 审批人名称
        if (StringUtils.hasText(header.getApproverId())) {
            try {
                AdmPractitioner approver = admPractitionerMapper.selectById(header.getApproverId());
                if (approver != null) {
                    vo.setApproverName(approver.getName());
                }
            } catch (Exception ignored) { /* ignore */ }
        }

        // ---- 总金额 ----
        BigDecimal totalAmount = BigDecimal.ZERO;
        if (rows != null) {
            for (WkfSupplyRequest row : rows) {
                if (row.getTotalPrice() != null) {
                    totalAmount = totalAmount.add(row.getTotalPrice());
                }
            }
        }
        vo.setTotalAmount(totalAmount);

        // ---- 明细行 ----
        if (rows != null && !rows.isEmpty()) {
            // 批量预加载药品/耗材定义
            List<String> medIds = new ArrayList<>();
            List<String> devIds = new ArrayList<>();
            for (WkfSupplyRequest r : rows) {
                if ("med_medication_definition".equals(r.getItemTable()) && StringUtils.hasText(r.getItemId())) {
                    medIds.add(r.getItemId());
                } else if ("adm_device_definition".equals(r.getItemTable()) && StringUtils.hasText(r.getItemId())) {
                    devIds.add(r.getItemId());
                }
            }
            Map<String, MedMedicationDefinition> medMap = new HashMap<>();
            Map<String, AdmDeviceDefinition> devMap = new HashMap<>();
            if (!medIds.isEmpty()) {
                medMedicationDefinitionMapper.selectBatchIds(medIds).forEach(m -> medMap.put(m.getId(), m));
            }
            if (!devIds.isEmpty()) {
                admDeviceDefinitionMapper.selectBatchIds(devIds).forEach(d -> devMap.put(d.getId(), d));
            }

            List<InboundReceiptDetailLineVO> lines = rows.stream().map(r -> {
                InboundReceiptDetailLineVO line = new InboundReceiptDetailLineVO();
                line.setId(r.getId());
                line.setItemTable(r.getItemTable());
                line.setItemId(r.getItemId());
                line.setItemQuantity(r.getItemQuantity());
                line.setUnitCode(r.getUnitCode());
                line.setUnitCodeDictText(r.getUnitCode());
                line.setLotNumber(r.getLotNumber());
                line.setTraceNo(r.getTraceNo());
                line.setPrice(r.getPrice());
                line.setTotalPrice(r.getTotalPrice());
                line.setOccurrenceTime(r.getOccurrenceTime());
                line.setStartTime(r.getStartTime());
                line.setEndTime(r.getEndTime());
                line.setRemake(r.getRemake());
                // 补名称/规格/厂家
                if ("med_medication_definition".equals(r.getItemTable())) {
                    MedMedicationDefinition med = medMap.get(r.getItemId());
                    if (med != null) {
                        line.setProductName(med.getName());
                        line.setSpecification(med.getTotalVolume());
                        line.setManufacturer(med.getManufacturerText());
                    }
                } else if ("adm_device_definition".equals(r.getItemTable())) {
                    AdmDeviceDefinition dev = devMap.get(r.getItemId());
                    if (dev != null) {
                        line.setProductName(dev.getName());
                        line.setSpecification(dev.getTotalVolume());
                        line.setManufacturer(dev.getManufacturerText());
                    }
                }
                return line;
            }).toList();
            vo.setLines(lines);
        } else {
            vo.setLines(List.of());
        }

        return vo;
    }

    // ==================== 旧版 validateSaveRequest（保留兼容） ====================

    private void validateSaveRequest(InboundReceiptSaveDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("药库进货单入参不能为空");
        }
        if (dto.getLines() == null || dto.getLines().isEmpty()) {
            throw new IllegalArgumentException("药库进货单明细不能为空");
        }
        for (int i = 0; i < dto.getLines().size(); i++) {
            InboundReceiptSaveDTO.InboundReceiptLineDTO line = dto.getLines().get(i);
            if (line == null) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行明细不能为空");
            }
            if (!StringUtils.hasText(line.getItemTable())) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行项目表不能为空");
            }
            if (!StringUtils.hasText(line.getItemId())) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行项目标识不能为空");
            }
            if (!StringUtils.hasText(line.getUnitCode())) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行单位编码不能为空");
            }
            if (line.getItemQuantity() == null || line.getItemQuantity().compareTo(BigDecimal.ZERO) <= 0) {
                throw new IllegalArgumentException("药库进货单第" + (i + 1) + "行数量必须大于0");
            }
        }
    }

    private List<WkfSupplyRequest> requireEditableRows(String busNo, Integer expectedVersion) {
        List<WkfSupplyRequest> rows = listActive(busNo);
        if (rows.isEmpty()) {
            throw new IllegalArgumentException("药库进货单不存在：" + busNo);
        }
        requireSameStatus(rows);
        requireAnyStatus(rows.get(0).getStatusEnum(), STATUS_PENDING_APPROVAL, STATUS_WITHDRAW, STATUS_REJECT);
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

    private String requireBusNoFromSaveRequest(InboundReceiptSaveRequestDTO dto) {
        if (!StringUtils.hasText(dto.getBusNo())) {
            throw new IllegalArgumentException("保存药库进货单时业务单号不能为空");
        }
        return dto.getBusNo();
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

    private static final DateTimeFormatter FORMATTER = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    @Resource
    private WkfSupplyDeliveryMapper wkfSupplyDeliveryMapper;
    @Resource
    private AdmLocationMapper admLocationMapper;
    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;
    @Resource
    private MedMedicationDefinitionMapper medMedicationDefinitionMapper;
    @Resource
    private AdmPractitionerMapper admPractitionerMapper;
    @Resource
    private AdmChargeItemDefinitionMapper admChargeItemDefinitionMapper;

    @Override
    @Transactional(rollbackFor = Exception.class)
    public PharmacyInventoryDeleteVO deleteStocktake(String busNo) {
        PharmacyInventoryDeleteVO resultVO = new PharmacyInventoryDeleteVO();
        // 1. 查询单据是否存在且未逻辑删除
        LambdaQueryWrapper<WkfSupplyRequest> queryWrapper = new LambdaQueryWrapper<>();
        queryWrapper.eq(WkfSupplyRequest::getBusNo, busNo);
        queryWrapper.eq(WkfSupplyRequest::getTypeEnum, "PHARMACY_STOCKTAKE");
        queryWrapper.eq(WkfSupplyRequest::getIsDeleted, false);
        queryWrapper.last("LIMIT 1");
        WkfSupplyRequest stocktake = this.getOne(queryWrapper);
        // 判断单据是否存在
        if (stocktake == null) {
            throw new RuntimeException("单据不存在或已经被删除");
        }
        // 2. 业务规则校验：已通过和审批中的单据不允许删除
        String status = stocktake.getStatusEnum();
        if ("APPROVAL".equals(status) || "AGREE".equals(status)) {
            throw new RuntimeException("当前状态不允许删除，当前状态: " + status);
        }
        // 3. 逻辑删除主单据主键ID更新
        LambdaUpdateWrapper<WkfSupplyRequest> updateWrapper = new LambdaUpdateWrapper<>();
        updateWrapper.eq(WkfSupplyRequest::getId, stocktake.getId());
        updateWrapper.set(WkfSupplyRequest::getIsDeleted, true);
        updateWrapper.set(WkfSupplyRequest::getDeletedAt, LocalDateTime.now());
        boolean updateMain = this.update(updateWrapper);
        // 4.删除该单据下所有盘点明细
        LambdaUpdateWrapper<WkfSupplyDelivery> deliveryUpdateWrapper = new LambdaUpdateWrapper<>();
        deliveryUpdateWrapper.eq(WkfSupplyDelivery::getRequestId, stocktake.getId());
        deliveryUpdateWrapper.eq(WkfSupplyDelivery::getIsDeleted, false);
        deliveryUpdateWrapper.set(WkfSupplyDelivery::getIsDeleted, true);
        deliveryUpdateWrapper.set(WkfSupplyDelivery::getDeletedAt, LocalDateTime.now());
        wkfSupplyDeliveryMapper.update(deliveryUpdateWrapper);
        resultVO.setSuccess(updateMain);
        return resultVO;
    }




    @Override
    public PharmacyInventoryDetailVO getStocktakeDetail(PharmacyInventoryDetailQuery query) {
        String stocktakeNo = query.getStocktakeNo();
        if (!StringUtils.hasText(stocktakeNo)) {
            throw new RuntimeException("盘点单据号不能为空");
        }
        // 查询所有明细行（从 wkf_supply_request 读）
        QueryWrapper<WkfSupplyRequest> reqWrapper = new QueryWrapper<>();
        reqWrapper.eq("bus_no", stocktakeNo);
        reqWrapper.eq("type_enum", "PHARMACY_STOCKTAKE");
        reqWrapper.eq("is_deleted", 0);
        List<WkfSupplyRequest> rows = baseMapper.selectList(reqWrapper);
        if (rows.isEmpty()) {
            throw new RuntimeException("盘点单据不存在或已删除");
        }
        WkfSupplyRequest header = rows.get(0);

        // 库房名称
        String locationName = null;
        if (StringUtils.hasText(header.getPurposeLocationId())) {
            AdmLocation location = admLocationMapper.selectById(header.getPurposeLocationId());
            if (location != null) locationName = location.getName();
        }

        // 批量查药品信息
        List<String> itemIds = rows.stream()
                .filter(r -> "med_medication_definition".equals(r.getItemTable()))
                .map(WkfSupplyRequest::getItemId).filter(StringUtils::hasText).distinct().toList();
        Map<String, MedMedicationDefinition> medMap = new HashMap<>();
        if (!itemIds.isEmpty()) {
            medMedicationDefinitionMapper.selectBatchIds(itemIds)
                    .forEach(m -> medMap.put(m.getId(), m));
        }

        // 组装
        PharmacyInventoryDetailVO detailVO = new PharmacyInventoryDetailVO();
        detailVO.setBusNo(header.getBusNo());
        detailVO.setStatusEnum(header.getStatusEnum());
        detailVO.setPurposeLocationId(header.getPurposeLocationId());
        detailVO.setPurposeLocationName(locationName);
        detailVO.setPractitionerId(header.getPractitionerId());
        detailVO.setApplicantId(header.getApplicantId());
        detailVO.setApplyTime(header.getApplyTime() != null ? header.getApplyTime().format(FORMATTER) : null);
        detailVO.setApproverId(header.getApproverId());
        detailVO.setApprovalTime(header.getApprovalTime() != null ? header.getApprovalTime().format(FORMATTER) : null);

        BigDecimal totalAmount = BigDecimal.ZERO;
        List<PharmacyInventoryItemVO> lineVoList = new ArrayList<>();
        for (WkfSupplyRequest r : rows) {
            if (!StringUtils.hasText(r.getItemId())) continue; // 跳过锚点行
            PharmacyInventoryItemVO itemVO = new PharmacyInventoryItemVO();
            itemVO.setLineId(r.getId());
            itemVO.setItemTable(r.getItemTable());
            itemVO.setItemId(r.getItemId());
            itemVO.setLotNumber(r.getLotNumber());
            itemVO.setUnitCode(r.getUnitCode());
            itemVO.setOccurrenceTime(r.getOccurrenceTime() != null ? r.getOccurrenceTime().format(FORMATTER) : null);
            itemVO.setActualQuantity(r.getTotalQuantity() != null ? r.getTotalQuantity() : BigDecimal.ZERO);
            itemVO.setProfitLossQuantity(r.getItemQuantity() != null ? r.getItemQuantity() : BigDecimal.ZERO);
            itemVO.setBookQuantity(itemVO.getActualQuantity().subtract(itemVO.getProfitLossQuantity()));
            itemVO.setPrice(r.getPrice() != null ? r.getPrice() : BigDecimal.ZERO);
            itemVO.setActualAmount(r.getTotalPrice() != null ? r.getTotalPrice() : BigDecimal.ZERO);
            itemVO.setProfitLossAmount(r.getProfitLossPrice() != null ? r.getProfitLossPrice() : BigDecimal.ZERO);
            if (r.getTotalPrice() != null) totalAmount = totalAmount.add(r.getTotalPrice());
            MedMedicationDefinition med = medMap.get(r.getItemId());
            if (med != null) {
                itemVO.setProductName(med.getName());
                itemVO.setSpecification(med.getTotalVolume());
                itemVO.setManufacturer(med.getManufacturerText());
            }
            lineVoList.add(itemVO);
        }
        detailVO.setTotalAmount(totalAmount);
        detailVO.setLines(lineVoList);
        return detailVO;
    }


    /**
     * 状态编码转换中文名称
     */
    private String convertStatusText(String statusEnum) {
        if (!StringUtils.hasText(statusEnum)) {
            return "";
        }
        return switch (statusEnum) {
            case "PENDING_APPROVAL" -> "待审核";
            case "APPROVAL" -> "审核中";
            case "AGREE" -> "已通过";
            case "REJECT" -> "已驳回";
            case "WITHDRAW" -> "已撤回";
            default -> "未知状态";
        };
    }


    @Override
    public PharmacyInventoryListVO listInventory(PharmacyInventoryQuery query) {
        //1.构建分页对象
        Page<WkfSupplyRequest> page = new Page<>();
        long pageIndex = query.getPageIndex() < 1 ? 1 : query.getPageIndex();
        long pageSize = query.getPageSize() < 1 ? 10 : query.getPageSize();
        page.setCurrent(pageIndex);
        page.setSize(pageSize);
//        page.setCurrent(query.getPageIndex());
//        page.setSize(query.getPageSize());
        // 2.构造主表查询条件
        var wrapper = new com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper<WkfSupplyRequest>();
        wrapper.eq(WkfSupplyRequest::getIsDeleted, false);
        wrapper.eq(WkfSupplyRequest::getTypeEnum, "PHARMACY_STOCKTAKE");
        // 单据号模糊查询 stocktakeNo → busNo
        if (StringUtils.hasText(query.getStocktakeNo())) {
            wrapper.like(WkfSupplyRequest::getBusNo, query.getStocktakeNo());
        }
        // 时间范围处理
        if (StringUtils.hasText(query.getStartDate())) {
            LocalDateTime start = LocalDateTime.parse(query.getStartDate() + " 00:00:00", FORMATTER);
            wrapper.ge(WkfSupplyRequest::getCreatedAt, start);
        }
        if (StringUtils.hasText(query.getEndDate())) {
            LocalDateTime end = LocalDateTime.parse(query.getEndDate() + " 23:59:59", FORMATTER);
            wrapper.le(WkfSupplyRequest::getCreatedAt, end);
        }
        // 库房名称 pharmacy 处理：名称查库获取locationId
        if (StringUtils.hasText(query.getPharmacy())) {
            com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper<AdmLocation> locWrapper
                    = new com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper<>();
            locWrapper.like(AdmLocation::getName, query.getPharmacy());
            List<AdmLocation> locationList = admLocationMapper.selectList(locWrapper);
            List<String> locIdList = locationList.stream()
                    .map(AdmLocation::getId)
                    .collect(Collectors.toList());
            if (!locIdList.isEmpty()) {
                wrapper.in(WkfSupplyRequest::getPurposeLocationId, locIdList);
            } else {
                // 没有匹配库房，直接返回空
//                PharmacyInventoryListVO resultVO = new PharmacyInventoryListVO();
//                resultVO.setContent(new ArrayList<>());
//                resultVO.setTotalCount(0L);
//                resultVO.setPageIndex(query.getPageIndex());
//                resultVO.setPageSize(query.getPageSize());
//                return resultVO;
                locIdList.clear();
            }
        }
        // 创建时间倒序，最新单据置顶
        wrapper.orderByDesc(WkfSupplyRequest::getCreatedAt);
        IPage<WkfSupplyRequest> reqPage = baseMapper.selectPage(page, wrapper);
        List<WkfSupplyRequest> records = reqPage.getRecords();
        PharmacyInventoryListVO resultVO = new PharmacyInventoryListVO();
        resultVO.setTotalCount(reqPage.getTotal());
        resultVO.setPageIndex(query.getPageIndex());
        resultVO.setPageSize(query.getPageSize());
        if (CollectionUtils.isEmpty(records)) {
            resultVO.setContent(new ArrayList<>());
            return resultVO;
        }
        // 3.收集ID批量查询附属信息
        Set<String> locationIdSet = new HashSet<>();
        Set<String> practitionerIdSet = new HashSet<>();
        List<String> requestIdList = new ArrayList<>();
        for (WkfSupplyRequest req : records) {
            if (StringUtils.hasText(req.getPurposeLocationId())) {
                locationIdSet.add(req.getPurposeLocationId());
            }
            if (StringUtils.hasText(req.getCreatedBy())) {
                practitionerIdSet.add(req.getCreatedBy());
            }
            if (StringUtils.hasText(req.getApproverId())) {
                practitionerIdSet.add(req.getApproverId());
            }
            requestIdList.add(req.getId());
        }

        // 查询库房名称
        Map<String, String> locationNameMap = new HashMap<>();
        if (!locationIdSet.isEmpty()) {
            List<AdmLocation> locationList = admLocationMapper.selectBatchIds(locationIdSet);
            locationNameMap = locationList.stream()
                    .collect(Collectors.toMap(AdmLocation::getId, AdmLocation::getName, (v1, v2) -> v1));
        }

        // 查询人员名称
        Map<String, String> practitionerNameMap = new HashMap<>();
        if (!practitionerIdSet.isEmpty()) {
            List<AdmPractitioner> practitionerList = admPractitionerMapper.selectBatchIds(practitionerIdSet);
            practitionerNameMap = practitionerList.stream()
                    .collect(Collectors.toMap(AdmPractitioner::getId, AdmPractitioner::getName, (v1, v2) -> v1));
        }

        // 4.批量统计明细条目数量（从 wkf_supply_request 自身行数）
        List<String> busNos = records.stream().map(WkfSupplyRequest::getBusNo).distinct().toList();
        Map<String, Long> itemCountMap = new HashMap<>();
        if (!busNos.isEmpty()) {
            QueryWrapper<WkfSupplyRequest> countWrapper = new QueryWrapper<>();
            countWrapper.select("bus_no, COUNT(*) AS cnt");
            countWrapper.in("bus_no", busNos);
            countWrapper.eq("type_enum", "PHARMACY_STOCKTAKE");
            countWrapper.eq("is_deleted", 0);
            countWrapper.ne("item_table", "");
            countWrapper.groupBy("bus_no");
            countWrapper.getSqlSelect(); // dummy to suppress warning
            // use selectMaps
            LambdaQueryWrapper<WkfSupplyRequest> cw = new LambdaQueryWrapper<>();
            cw.in(WkfSupplyRequest::getBusNo, busNos);
            cw.eq(WkfSupplyRequest::getTypeEnum, "PHARMACY_STOCKTAKE");
            cw.eq(WkfSupplyRequest::getIsDeleted, false);
            cw.ne(WkfSupplyRequest::getItemTable, "");
            List<Map<String, Object>> countRows = baseMapper.selectMaps(cw);
            for (Map<String, Object> row : countRows) {
                String bn = (String) row.get("bus_no");
                itemCountMap.merge(bn, 1L, Long::sum);
            }
        }

        // 5.实体转VO
        List<PharmacyInventoryVO> voContent = new ArrayList<>();
        for (WkfSupplyRequest req : records) {
            PharmacyInventoryVO vo = new PharmacyInventoryVO();
            vo.setBusNo(req.getBusNo());
            vo.setStatusEnum(req.getStatusEnum());
            vo.setStatusEnumText(convertStatusText(req.getStatusEnum()));
            vo.setPurposeLocationId(req.getPurposeLocationId());
            vo.setPurposeLocationName(locationNameMap.get(req.getPurposeLocationId()));
            // 明细条目数量
            vo.setItemCount(itemCountMap.getOrDefault(req.getBusNo(), 0L).intValue());

            // 盈亏金额、实际总金额（主表读取）
            vo.setProfitLossQuantitySum(req.getProfitLossPrice() == null ? BigDecimal.ZERO : req.getProfitLossPrice());
            vo.setActualAmountSum(req.getTotalPrice() == null ? BigDecimal.ZERO : req.getTotalPrice());
            // 申请人
            vo.setApplicantId(req.getApplicantId());
            vo.setApplicantName(practitionerNameMap.get(req.getApplicantId()));
            if (req.getApplyTime() != null) {
                vo.setApplyTime(req.getApplyTime().format(FORMATTER));
            } else if (req.getCreatedAt() != null) {
                vo.setApplyTime(req.getCreatedAt().format(FORMATTER));
            }
            // 审批人
            vo.setApproverId(req.getApproverId());
            vo.setApproverName(practitionerNameMap.get(req.getApproverId()));
            vo.setApprovalTime(req.getApprovalTime() == null ? null : req.getApprovalTime().format(FORMATTER));
            // 盘点发生时间
            vo.setOccurrenceTime(req.getOccurrenceTime() == null ? null : req.getOccurrenceTime().format(FORMATTER));
            voContent.add(vo);
        }
        resultVO.setContent(voContent);
        return resultVO;
    }

    /**
     * 内部DTO，接收明细统计数量
     */
    @lombok.Data
    public static class RequestSummaryDTO {
        private String requestId;
        private Integer itemCount;
    }

}
