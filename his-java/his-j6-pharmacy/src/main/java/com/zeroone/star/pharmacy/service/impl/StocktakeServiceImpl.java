package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.zeroone.star.pharmacy.entity.AdmLocation;
import com.zeroone.star.pharmacy.entity.AdmPractitioner;
import com.zeroone.star.pharmacy.entity.MedMedicationDefinition;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.AdmLocationMapper;
import com.zeroone.star.pharmacy.mapper.AdmPractitionerMapper;
import com.zeroone.star.pharmacy.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeAddDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSaveRequestDTO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.ThreadLocalRandom;

/**
 * 药库盘点单 核心业务实现。
 */
@Service
public class StocktakeServiceImpl implements IStocktakeService {

    private static final String TYPE_WAREHOUSE_STOCKTAKE = "PHARMACY_STOCKTAKE";
    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";

    @Resource
    private WkfSupplyRequestMapper supplyRequestMapper;

    @Resource
    private WkfInventoryItemMapper inventoryItemMapper;

    @Resource
    private MedMedicationDefinitionMapper medicationMapper;

    @Resource
    private AdmLocationMapper locationMapper;

    @Resource
    private AdmPractitionerMapper practitionerMapper;

    @Resource
    private UserHolder userHolder;

    // ==================== 1. 新增盘点单 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeDetailVO createStocktake(StocktakeAddDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("请求参数不能为空");
        }
        if (!StringUtils.hasText(dto.getPurposeLocationId())) {
            throw new IllegalArgumentException("库房位置不能为空");
        }

        // 库房校验
        AdmLocation location = locationMapper.selectById(dto.getPurposeLocationId());
        if (location == null) {
            throw new IllegalArgumentException("库房位置不存在: " + dto.getPurposeLocationId());
        }

        String busNo = generateBusNo();
        String userId = currentUserId();
        String practitionerId = StringUtils.hasText(dto.getPractitionerId())
                ? dto.getPractitionerId() : userId;
        LocalDateTime now = LocalDateTime.now();

        // 插入锚点行（无明细）
        WkfSupplyRequest anchor = new WkfSupplyRequest();
        anchor.setId(UUID.randomUUID().toString().replace("-", ""));
        anchor.setBusNo(busNo);
        anchor.setTypeEnum(TYPE_WAREHOUSE_STOCKTAKE);
        anchor.setStatusEnum(STATUS_DRAFT);
        anchor.setCategoryEnum(dto.getCategoryEnum());
        anchor.setPurposeLocationId(dto.getPurposeLocationId());
        anchor.setPractitionerId(practitionerId);
        anchor.setApplicantId(userId);
        anchor.setCreatedAt(now);
        anchor.setCreatedBy(userId);
        anchor.setUpdatedAt(now);
        anchor.setUpdatedBy(userId);
        anchor.setIsDeleted(false);
        anchor.setVersion(0);
        // NOT NULL 占位
        anchor.setItemTable("");
        anchor.setItemId("");
        anchor.setItemQuantity(BigDecimal.ZERO);
        anchor.setUnitCode("");
        supplyRequestMapper.insert(anchor);

        return buildEnrichedVO(anchor, new ArrayList<>());
    }

    // ==================== 2. 保存盘点单 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeDetailVO saveStocktake(String busNo, StocktakeSaveRequestDTO dto) {
        if (!StringUtils.hasText(busNo)) {
            throw new IllegalArgumentException("busNo 不能为空");
        }
        if (dto == null || dto.getLines() == null || dto.getLines().isEmpty()) {
            throw new IllegalArgumentException("保存时明细行不能为空");
        }

        // 校验行字段
        assertStocktakeLinesFields(dto.getLines());

        // 查已有行
        QueryWrapper<WkfSupplyRequest> queryWrapper = new QueryWrapper<>();
        queryWrapper.eq("bus_no", busNo);
        queryWrapper.eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE);
        queryWrapper.eq("is_deleted", 0);
        List<WkfSupplyRequest> existingRows = supplyRequestMapper.selectList(queryWrapper);
        if (existingRows.isEmpty()) {
            throw new IllegalArgumentException("盘点单不存在: " + busNo);
        }
        String currentStatus = existingRows.get(0).getStatusEnum();
        if (!STATUS_DRAFT.equals(currentStatus)) {
            throw new IllegalArgumentException("当前状态不允许编辑，当前状态: " + currentStatus);
        }

        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        // 构建明细行
        List<StocktakeDetailVO.StocktakeDetailLineVO> detailLines = buildDetailLines(dto.getLines(), now);

        // 软删旧行
        UpdateWrapper<WkfSupplyRequest> deleteWrapper = new UpdateWrapper<>();
        deleteWrapper.eq("bus_no", busNo);
        deleteWrapper.eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE);
        deleteWrapper.eq("is_deleted", 0);
        deleteWrapper.set("is_deleted", 1);
        deleteWrapper.set("deleted_at", now);
        deleteWrapper.set("deleted_by", userId);
        supplyRequestMapper.update(null, deleteWrapper);

        // 插入新行
        for (StocktakeDetailVO.StocktakeDetailLineVO lineVO : detailLines) {
            WkfSupplyRequest row = new WkfSupplyRequest();
            row.setId(UUID.randomUUID().toString().replace("-", ""));
            row.setBusNo(busNo);
            row.setTypeEnum(TYPE_WAREHOUSE_STOCKTAKE);
            row.setStatusEnum(currentStatus);
            row.setCategoryEnum(dto.getCategoryEnum());
            row.setPurposeLocationId(dto.getPurposeLocationId());
            row.setPractitionerId(dto.getPractitionerId());
            row.setItemTable(lineVO.getItemTable());
            row.setItemId(lineVO.getItemId());
            row.setItemQuantity(lineVO.getProfitLossQuantity());
            row.setTotalQuantity(lineVO.getActualQuantity());
            row.setUnitCode(lineVO.getUnitCode());
            row.setLotNumber(lineVO.getLotNumber());
            row.setPrice(lineVO.getPrice());
            row.setTotalPrice(lineVO.getActualAmount());
            row.setProfitLossPrice(lineVO.getProfitLossAmount());
            row.setSupplierId(lineVO.getSupplierId());
            row.setTraceNo(lineVO.getTraceNo());
            row.setRemake(lineVO.getRemake());
            row.setOccurrenceTime(now);
            row.setApplicantId(userId);
            row.setCreatedAt(now);
            row.setCreatedBy(userId);
            row.setUpdatedAt(now);
            row.setUpdatedBy(userId);
            row.setIsDeleted(false);
            row.setVersion(0);
            supplyRequestMapper.insert(row);
        }

        // 组装 VO
        StocktakeDetailVO vo = buildEnrichedVOFromLines(busNo, currentStatus, dto, detailLines);
        return vo;
    }

    // ==================== 3. 保存并提交审批 ====================

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeDetailVO submitStocktake(String busNo, StocktakeSaveRequestDTO dto) {
        // 执行保存
        StocktakeDetailVO vo = saveStocktake(busNo, dto);

        // 状态翻转
        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        UpdateWrapper<WkfSupplyRequest> updateWrapper = new UpdateWrapper<>();
        updateWrapper.eq("bus_no", busNo);
        updateWrapper.eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE);
        updateWrapper.eq("is_deleted", 0);
        updateWrapper.set("status_enum", STATUS_PENDING_APPROVAL);
        updateWrapper.set("applicant_id", userId);
        updateWrapper.set("apply_time", now);
        updateWrapper.set("updated_at", now);
        updateWrapper.set("updated_by", userId);
        supplyRequestMapper.update(null, updateWrapper);

        vo.setStatusEnum(STATUS_PENDING_APPROVAL);
        vo.setApplicantId(userId);
        vo.setApplyTime(now);
        return vo;
    }

    // ==================== 辅助方法 ====================

    private StocktakeDetailVO buildEnrichedVO(WkfSupplyRequest header,
                                              List<StocktakeDetailVO.StocktakeDetailLineVO> lines) {
        StocktakeDetailVO vo = new StocktakeDetailVO();
        vo.setBusNo(header.getBusNo());
        vo.setStatusEnum(header.getStatusEnum());
        vo.setPurposeLocationId(header.getPurposeLocationId());
        vo.setPractitionerId(header.getPractitionerId());
        vo.setCreatorId(header.getCreatedBy());
        vo.setApplicantId(header.getApplicantId());
        vo.setApproverId(header.getApproverId());
        vo.setApplyTime(header.getApplyTime());
        vo.setApprovalTime(header.getApprovalTime());
        vo.setCategoryEnum(header.getCategoryEnum());

        // 名称解析
        resolveNames(vo);

        // 总金额
        BigDecimal total = BigDecimal.ZERO;
        if (lines != null) {
            for (StocktakeDetailVO.StocktakeDetailLineVO line : lines) {
                if (line.getActualAmount() != null) {
                    total = total.add(line.getActualAmount());
                }
            }
        }
        vo.setTotalAmount(total);
        vo.setLines(lines != null ? lines : new ArrayList<>());
        return vo;
    }

    private StocktakeDetailVO buildEnrichedVOFromLines(String busNo, String status,
                                                        StocktakeSaveRequestDTO dto,
                                                        List<StocktakeDetailVO.StocktakeDetailLineVO> detailLines) {
        StocktakeDetailVO vo = new StocktakeDetailVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum(status);
        vo.setPurposeLocationId(dto.getPurposeLocationId());
        vo.setPractitionerId(dto.getPractitionerId());
        vo.setCategoryEnum(dto.getCategoryEnum());
        vo.setLines(detailLines);

        // 名称解析
        resolveNames(vo);

        // 总金额
        BigDecimal total = BigDecimal.ZERO;
        for (StocktakeDetailVO.StocktakeDetailLineVO line : detailLines) {
            if (line.getActualAmount() != null) {
                total = total.add(line.getActualAmount());
            }
        }
        vo.setTotalAmount(total);
        return vo;
    }

    private void resolveNames(StocktakeDetailVO vo) {
        // 库房名称
        if (StringUtils.hasText(vo.getPurposeLocationId())) {
            try {
                AdmLocation loc = locationMapper.selectById(vo.getPurposeLocationId());
                if (loc != null) vo.setPurposeLocationName(loc.getName());
            } catch (Exception ignored) {}
        }
        // 盘点人名称
        if (StringUtils.hasText(vo.getPractitionerId())) {
            try {
                AdmPractitioner p = practitionerMapper.selectById(vo.getPractitionerId());
                if (p != null) vo.setPractitionerName(p.getName());
            } catch (Exception ignored) {}
        }
        // 制单人名称
        if (StringUtils.hasText(vo.getCreatorId())) {
            try {
                AdmPractitioner p = practitionerMapper.selectById(vo.getCreatorId());
                if (p != null) vo.setCreatorName(p.getName());
            } catch (Exception ignored) {}
        }
        // 申请人名称
        if (StringUtils.hasText(vo.getApplicantId())) {
            try {
                AdmPractitioner p = practitionerMapper.selectById(vo.getApplicantId());
                if (p != null) vo.setApplicantName(p.getName());
            } catch (Exception ignored) {}
        }
        // 审批人名称
        if (StringUtils.hasText(vo.getApproverId())) {
            try {
                AdmPractitioner p = practitionerMapper.selectById(vo.getApproverId());
                if (p != null) vo.setApproverName(p.getName());
            } catch (Exception ignored) {}
        }
    }

    private List<StocktakeDetailVO.StocktakeDetailLineVO> buildDetailLines(
            List<StocktakeSaveRequestDTO.StocktakeLineDTO> lineDTOs, LocalDateTime now) {
        List<StocktakeDetailVO.StocktakeDetailLineVO> result = new ArrayList<>();
        for (StocktakeSaveRequestDTO.StocktakeLineDTO lineDTO : lineDTOs) {
            WkfInventoryItem item = inventoryItemMapper.selectById(lineDTO.getInventoryItemId());

            BigDecimal bookQuantity = item.getQuantity() != null ? item.getQuantity() : BigDecimal.ZERO;
            String productName = item.getName();
            String specification = null;
            String manufacturer = null;
            BigDecimal price = BigDecimal.ZERO;

            if ("med_medication_definition".equals(item.getItemTable())) {
                MedMedicationDefinition med = medicationMapper.selectById(item.getItemId());
                if (med != null) {
                    specification = med.getComprisedText();
                    manufacturer = med.getManufacturerText();
                }
            }

            BigDecimal profitLossQuantity = lineDTO.getProfitLossQuantity() != null
                    ? lineDTO.getProfitLossQuantity() : BigDecimal.ZERO;
            BigDecimal actualQuantity = lineDTO.getActualQuantity() != null
                    ? lineDTO.getActualQuantity() : BigDecimal.ZERO;
            BigDecimal actualAmount = actualQuantity.multiply(price);
            BigDecimal profitLossAmount = profitLossQuantity.multiply(price);

            StocktakeDetailVO.StocktakeDetailLineVO lineVO = new StocktakeDetailVO.StocktakeDetailLineVO();
            lineVO.setLineId(UUID.randomUUID().toString().replace("-", ""));
            lineVO.setInventoryItemId(lineDTO.getInventoryItemId());
            lineVO.setItemTable(item.getItemTable());
            lineVO.setItemId(item.getItemId());
            lineVO.setProductName(productName);
            lineVO.setSpecification(specification);
            lineVO.setManufacturer(manufacturer);
            lineVO.setLotNumber(item.getLotNumber());
            lineVO.setExpirationDate(item.getExpirationDate() != null ? item.getExpirationDate().toString() : null);
            lineVO.setUnitCode(item.getUnitCode());
            lineVO.setBookQuantity(bookQuantity);
            lineVO.setProfitLossQuantity(profitLossQuantity);
            lineVO.setActualQuantity(actualQuantity);
            lineVO.setActualAmount(actualAmount);
            lineVO.setProfitLossAmount(profitLossAmount);
            lineVO.setPrice(price);
            lineVO.setSupplierId(item.getSupplierId());
            lineVO.setTraceNo(lineDTO.getTraceNo());
            lineVO.setRemake(lineDTO.getRemake());
            lineVO.setOccurrenceTime(now);
            result.add(lineVO);
        }
        return result;
    }

    private void assertStocktakeLinesFields(List<StocktakeSaveRequestDTO.StocktakeLineDTO> lines) {
        if (lines == null || lines.isEmpty()) {
            throw new IllegalArgumentException("明细行不能为空");
        }
        Set<String> ALLOWED_TABLES = Set.of("med_medication_definition", "adm_device_definition");
        for (int i = 0; i < lines.size(); i++) {
            StocktakeSaveRequestDTO.StocktakeLineDTO line = lines.get(i);
            String prefix = "第" + (i + 1) + "行: ";

            if (!StringUtils.hasText(line.getInventoryItemId())) {
                throw new IllegalArgumentException(prefix + "库存项ID不能为空");
            }
            if (!StringUtils.hasText(line.getUnitCode())) {
                throw new IllegalArgumentException(prefix + "计量单位编码不能为空");
            }
            if (line.getActualQuantity() == null) {
                throw new IllegalArgumentException(prefix + "实盘数量不能为空");
            }
            if (line.getActualQuantity().compareTo(BigDecimal.ZERO) < 0) {
                throw new IllegalArgumentException(prefix + "实盘数量不能为负数");
            }

            WkfInventoryItem item = inventoryItemMapper.selectById(line.getInventoryItemId());
            if (item == null) {
                throw new IllegalArgumentException(prefix + "库存项不存在: " + line.getInventoryItemId());
            }
            if (!StringUtils.hasText(item.getItemTable())) {
                throw new IllegalArgumentException(prefix + "库存项的物品定义表名为空");
            }
            if (!ALLOWED_TABLES.contains(item.getItemTable())) {
                throw new IllegalArgumentException(prefix + "不支持的物品类型: " + item.getItemTable()
                        + "，仅允许: " + ALLOWED_TABLES);
            }
        }
    }

    private String currentUserId() {
        try {
            UserDTO currentUser = userHolder.getCurrentUser();
            if (currentUser != null && StringUtils.hasText(currentUser.getId())) {
                return currentUser.getId();
            }
        } catch (Exception ignored) {}
        return "system";
    }

    private String generateBusNo() {
        String dateStr = new SimpleDateFormat("yyyyMMdd").format(new Date());
        int seq = ThreadLocalRandom.current().nextInt(1000, 10000);
        return "CST" + dateStr + String.format("%04d", seq);
    }
}
