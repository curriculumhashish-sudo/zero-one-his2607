package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import com.zeroone.star.pharmacy.entity.AdmLocation;
import com.zeroone.star.pharmacy.entity.MedMedicationDefinition;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.AdmLocationMapper;
import com.zeroone.star.pharmacy.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.pharmacy.service.IWkfInventoryItemService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.StocktakeDetailVO;
import jakarta.annotation.Resource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.util.*;
import java.util.concurrent.ThreadLocalRandom;

/**
 * 药库盘点单 核心业务实现
 * <p>负责盘点单的新增、保存、保存并提交审批</p>
 *
 * @author Dome
 * @since 2026-07-24
 */
@Service
public class StocktakeServiceImpl implements IStocktakeService {

    private static final String TYPE_WAREHOUSE_STOCKTAKE = "WAREHOUSE_STOCKTAKE";
    private static final String STATUS_SUBMITTED = "SUBMITTED";
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
    private UserHolder userHolder;
   

    // ==================== 1. 新增盘点单 ====================

    /**
     * 新增盘点单
     * <p>生成空盘点单头（wkf_supply_request），不生成明细行</p>
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeDetailVO createStocktake(StocktakeSaveDTO dto) {
        // --- Gate ---
        if(dto == null){
            throw new IllegalArgumentException("请求参数不能为空");
        }
        if(!StringUtils.hasText(dto.getPurposeLocationId())){
            throw new IllegalArgumentException("库房位置不能为空");
        }
        if(dto.getLines() == null || dto.getLines().isEmpty()){
            throw new IllegalArgumentException("明细行不能为空");
        }

        // -- 库房位置校验 --
        AdmLocation location = locationMapper.selectById(dto.getPurposeLocationId());
        if (location == null){
            throw new IllegalArgumentException("库房位置不存在" + dto.getPurposeLocationId());
        }

        // --- 明细行字段校验--
        assertStocktakeLinesFields(dto.getLines());

        // --- 生成 busNo ---
        String busNo = generateBusNo();

        // --- 获取当前用户 ---
        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        // ---逐行查询库存项 -> 构建实体 -> 批量insert ---
        List<WkfSupplyRequest> entities = new  ArrayList<>();
        for (StocktakeSaveDTO.StocktakeLineDTO lineDTO : dto.getLines()) {
            WkfInventoryItem item = inventoryItemMapper.selectById(lineDTO.getInventoryItemId());
            
            WkfSupplyRequest row =  new WkfSupplyRequest();
            row.setId(UUID.randomUUID().toString().replace("-",""));
            row.setBusNo(busNo);
            row.setTypeEnum(TYPE_WAREHOUSE_STOCKTAKE);
            row.setStatusEnum(STATUS_PENDING_APPROVAL);
            row.setCategoryEnum(dto.getCategoryEnum());
            row.setPurposeLocationId(dto.getPurposeLocationId());
            row.setPractitionerId(dto.getPractitionerId());
            row.setItemTable(item.getItemTable());
            row.setItemId(item.getItemId());
            row.setItemQuantity(lineDTO.getProfitLossQuantity());
            row.setTotalQuantity(lineDTO.getActualQuantity());
            row.setUnitCode(lineDTO.getUnitCode() != null ? lineDTO.getUnitCode() : item.getUnitCode());
            row.setLotNumber(lineDTO.getLotNumber() != null ? lineDTO.getLotNumber() : item.getLotNumber());
            row.setTraceNo(lineDTO.getTraceNo());
            row.setRemake(lineDTO.getRemake());
            row.setOccurrenceTime(now);
            row.setApplicantId(userId);
            row.setCreatedAt(now);
            row.setCreatedBy(userId);
            row.setUpdatedAt(now);
            row.setUpdatedBy(userId);
            row.setIsDeleted(false);
            row.setVersion(0);
            entities.add(row);
        }
        for (WkfSupplyRequest row : entities) {
            supplyRequestMapper.insert(row);
        }
        StocktakeDetailVO vo =  new StocktakeDetailVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum(STATUS_PENDING_APPROVAL);
        vo.setPurposeLocationId(dto.getPurposeLocationId());
        vo.setPurposeLocationName(location.getName());
        vo.setPractitionerId(dto.getPractitionerId());
        vo.setCategoryEnum(dto.getCategoryEnum());
        
        List<StocktakeDetailVO.StocktakeDetailLineVO> lineVOs = new ArrayList<>();
        for (int i = 0; i < dto.getLines().size(); i++) {
            StocktakeSaveDTO.StocktakeLineDTO lineDTO = dto.getLines().get(i);
            WkfInventoryItem item = inventoryItemMapper.selectById(lineDTO.getInventoryItemId());

            StocktakeDetailVO.StocktakeDetailLineVO lineVO = new StocktakeDetailVO.StocktakeDetailLineVO();
            lineVO.setLineId(UUID.randomUUID().toString().replace("-", ""));
            lineVO.setInventoryItemId(lineDTO.getInventoryItemId());
            lineVO.setItemTable(item.getItemTable());
            lineVO.setItemId(item.getItemId());
            lineVO.setProductName(item.getName());
            lineVO.setUnitCode(lineDTO.getUnitCode() != null ? lineDTO.getUnitCode() : item.getUnitCode());
            lineVO.setLotNumber(lineDTO.getLotNumber() != null ? lineDTO.getLotNumber() : item.getLotNumber());
            lineVO.setExpirationDate(item.getExpirationDate() != null ? item.getExpirationDate().toString() : null);
            lineVO.setBookQuantity(item.getQuantity());
            lineVO.setProfitLossQuantity(lineDTO.getProfitLossQuantity());
            lineVO.setActualQuantity(lineDTO.getActualQuantity());
            lineVO.setSupplierId(item.getSupplierId());
            lineVO.setTraceNo(lineDTO.getTraceNo());
            lineVO.setRemake(lineDTO.getRemake());
            lineVO.setOccurrenceTime(now);
            lineVOs.add(lineVO);
        }
        vo.setLines(lineVOs);
        return vo;
    }

    // ==================== 2. 保存盘点单 ====================

    /**
     * 保存盘点单
     * <p>先删后插策略：快照锁定账面数 → 补全物品信息 → 批量写入明细行</p>
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeDetailVO saveStocktake(String busNo, StocktakeSaveDTO dto) {
        if(!StringUtils.hasText(busNo)){
            throw new IllegalArgumentException("busNO 不能为空");
        }
        if(dto == null || dto.getLines() == null || dto.getLines().isEmpty()){
            throw new IllegalArgumentException("保存时明细行不能为空");
        }

        QueryWrapper<WkfSupplyRequest> queryWrapper = new QueryWrapper<>();
        queryWrapper.eq("bus_no",busNo);
        queryWrapper.eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE);
        queryWrapper.eq("is_deleted",0);
        List<WkfSupplyRequest> existingRows = supplyRequestMapper.selectList(queryWrapper);
        if(existingRows.isEmpty()){
            throw new IllegalArgumentException("盘点单不存在:" + busNo);
        }
        String currentStatus = existingRows.get(0).getStatusEnum();
        if(!STATUS_PENDING_APPROVAL.equals(currentStatus)){
            throw new IllegalArgumentException("当前状态不允许编辑，当前状态: " + currentStatus);
        }

        // --- 明细行字段校验（已实现） ---
        assertStocktakeLinesFields(dto.getLines());

        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        // --- 遍历明细行：快照账面数 + 补全物品信息 ---
        List<StocktakeDetailVO.StocktakeDetailLineVO> detailLines = new ArrayList<>();
        for (StocktakeSaveDTO.StocktakeLineDTO lineDTO : dto.getLines()) {
            WkfInventoryItem item = inventoryItemMapper.selectById(lineDTO.getInventoryItemId());
            // item == null 已在 assertStocktakeLinesFields 中校验

            BigDecimal bookQuantity = item.getQuantity() != null ? item.getQuantity() : BigDecimal.ZERO;
            String productName = item.getName();
            String specification = null;
            String manufacturer = null;
            BigDecimal price = BigDecimal.ZERO;

            // 按 itemTable 分派查询物品定义表，补全展示字段
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
            BigDecimal profitLossAmount = profitLossQuantity.multiply(price);
            BigDecimal actualAmount = actualQuantity.multiply(price);

            StocktakeDetailVO.StocktakeDetailLineVO lineVO = new StocktakeDetailVO.StocktakeDetailLineVO();
            lineVO.setLineId(UUID.randomUUID().toString().replace("-", ""));
            lineVO.setInventoryItemId(lineDTO.getInventoryItemId());
            lineVO.setItemTable(item.getItemTable());
            lineVO.setItemId(item.getItemId());
            lineVO.setProductName(productName);
            lineVO.setSpecification(specification);
            lineVO.setManufacturer(manufacturer);
            lineVO.setLotNumber(lineDTO.getLotNumber() != null ? lineDTO.getLotNumber() : item.getLotNumber());
            lineVO.setExpirationDate(item.getExpirationDate() != null ? item.getExpirationDate().toString() : null);
            lineVO.setUnitCode(lineDTO.getUnitCode() != null ? lineDTO.getUnitCode() : item.getUnitCode());
            lineVO.setBookQuantity(bookQuantity);
            lineVO.setProfitLossQuantity(profitLossQuantity);
            lineVO.setActualQuantity(actualQuantity);
            lineVO.setProfitLossAmount(profitLossAmount);
            lineVO.setActualAmount(actualAmount);
            lineVO.setPrice(price);
            lineVO.setSupplierId(item.getSupplierId());
            lineVO.setTraceNo(lineDTO.getTraceNo());
            lineVO.setRemake(lineDTO.getRemake());
            lineVO.setOccurrenceTime(now);
            detailLines.add(lineVO);
        }

        // --- 逻辑删除旧行 ---
        UpdateWrapper<WkfSupplyRequest> deleteWrapper = new UpdateWrapper<>();
        deleteWrapper.eq("bus_no", busNo);
        deleteWrapper.eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE);
        deleteWrapper.eq("is_deleted", 0);
        deleteWrapper.set("is_deleted", 1);
        deleteWrapper.set("deleted_at", now);
        deleteWrapper.set("deleted_by", userId);
        supplyRequestMapper.update(null, deleteWrapper);


        // --- 逐行 INSERT 新明细 ---
        for (StocktakeDetailVO.StocktakeDetailLineVO lineVO : detailLines) {
            WkfSupplyRequest row = new WkfSupplyRequest();
            row.setId(UUID.randomUUID().toString().replace("-", ""));
            row.setBusNo(busNo);
            row.setTypeEnum(TYPE_WAREHOUSE_STOCKTAKE);
            row.setStatusEnum(STATUS_PENDING_APPROVAL);
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

        // --- 组装 VO ---
        StocktakeDetailVO vo = new StocktakeDetailVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum(STATUS_PENDING_APPROVAL);
        vo.setPurposeLocationId(dto.getPurposeLocationId());
        if (StringUtils.hasText(dto.getPurposeLocationId())) {
            AdmLocation loc = locationMapper.selectById(dto.getPurposeLocationId());
            vo.setPurposeLocationName(loc != null ? loc.getName() : "");
        }
        vo.setPractitionerId(dto.getPractitionerId());
        vo.setCategoryEnum(dto.getCategoryEnum());
        vo.setLines(detailLines);
        return vo;
    }

    // ==================== 3. 保存盘点单并提交审批 ====================

    /**
     * 保存盘点单并提交审批
     * <p>保存盘点数据 + 状态流转 DRAFT → PENDING_APPROVAL</p>
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeDetailVO submitStocktake(String busNo, StocktakeSaveDTO dto) {
        // --- Gate ---
        if (!StringUtils.hasText(busNo)) {
            throw new IllegalArgumentException("busNo 不能为空");
        }
        if (dto == null || dto.getLines() == null || dto.getLines().isEmpty()) {
            throw new IllegalArgumentException("提交时明细行不能为空");
        }

        // --- 执行保存逻辑（快照 + 补全 + 删旧插新） ---
        StocktakeDetailVO vo = saveStocktake(busNo, dto);

        // --- 状态翻转为 SUBMITTED ---
        String userId = currentUserId();
        LocalDateTime now = LocalDateTime.now();

        UpdateWrapper<WkfSupplyRequest> updateWrapper = new UpdateWrapper<>();
        updateWrapper.eq("bus_no", busNo);
        updateWrapper.eq("type_enum", TYPE_WAREHOUSE_STOCKTAKE);
        updateWrapper.eq("is_deleted", 0);
        updateWrapper.set("status_enum", STATUS_SUBMITTED);
        updateWrapper.set("applicant_id", userId);
        updateWrapper.set("apply_time", now);
        updateWrapper.set("updated_at", now);
        updateWrapper.set("updated_by", userId);
        int affected = supplyRequestMapper.update(null, updateWrapper);
        if (affected <= 0) {
            throw new IllegalArgumentException("提交审批失败，无符合条件的行");
        }

        // --- 更新 VO 状态 ---
        vo.setStatusEnum(STATUS_SUBMITTED);
        vo.setApplicantId(userId);
        vo.setApplyTime(now);
        return vo;
    }

    // ==================== 辅助方法 ====================

    /**
     * 获取当前登录用户ID，本地调用无网关 header 时返回 “system”
     */
    private String currentUserId(){
        try{
            UserDTO currentUser = userHolder.getCurrentUser();
            if(currentUser != null && StringUtils.hasText(currentUser.getId())){
                return currentUser.getId();
            }
        }catch (Exception e){
            //无网关 header 时容错
        }
        return "system";
    }

    /**
     * 明细行字段校验：白名单，必填项，数值合理性
     */
    private void assertStocktakeLinesFields(List<StocktakeSaveDTO.StocktakeLineDTO> lines){
        if (lines == null || lines.isEmpty()) {
            throw new IllegalArgumentException("明细行不能为空");
        }

        Set<String> ALLOWED_TABLES = Set.of("med_medication_definition", "adm_device_definition");
        for (int i = 0; i < lines.size(); i++) {
            StocktakeSaveDTO.StocktakeLineDTO line = lines.get(i);
            String prefix = "第" + (i + 1) + "行: ";

            // 1. 必填项校验
            if (!StringUtils.hasText(line.getInventoryItemId())) {
                throw new IllegalArgumentException(prefix + "库存项ID不能为空");
            }
            if (!StringUtils.hasText(line.getUnitCode())) {
                throw new IllegalArgumentException(prefix + "计量单位编码不能为空");
            }

            // 2. 数值合理性校验
            if (line.getActualQuantity() == null) {
                throw new IllegalArgumentException(prefix + "实盘数量不能为空");
            }
            if (line.getActualQuantity().compareTo(BigDecimal.ZERO) < 0) {
                throw new IllegalArgumentException(prefix + "实盘数量不能为负数");
            }
            if (line.getProfitLossQuantity() == null) {
                throw new IllegalArgumentException(prefix + "盈亏数量不能为空");
            }

            // 3. 白名单：查询库存项获取 itemTable，校验物品类型
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

    /**
     * 生成盘点单号: "CST" + yyyyMMdd + 4位数字
     */
    private String generateBusNo() {
        String dateStr = new SimpleDateFormat("yyyyMMdd").format(new Date());
        int seq = ThreadLocalRandom.current().nextInt(1000, 10000);
        return "CST" + dateStr + String.format("%04d", seq);
    }
}
