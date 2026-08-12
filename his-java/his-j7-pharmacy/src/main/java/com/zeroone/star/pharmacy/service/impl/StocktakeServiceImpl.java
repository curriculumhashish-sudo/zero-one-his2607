package com.zeroone.star.pharmacy.service.impl;

import com.alibaba.nacos.common.utils.StringUtils;
import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.mapper.StocktakeMapper;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.zeroone.star.project.components.user.UserDTO;
import com.zeroone.star.project.components.user.UserHolder;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.approval.OperatorDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeItemSaveDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeItemVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveVO;
import com.zeroone.star.pharmacy.mapper.InventoryLockMapper;
import com.zeroone.star.project.dto.j7.pharmacy.StocktakeDeleteDTO;
import com.zeroone.star.project.vo.j7.pharmacy.SupplyDocumentDeleteVO;
import jakarta.annotation.Resource;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Collections;
import java.util.List;

/**
 * <p>
 * 描述：盘点单新增保存、查询和提交服务实现
 * </p>
 * <p>
 * 功能说明：基于 {@code wkf_supply_request} 表实现盘点单业务。
 * 同一 {@code bus_no} 下聚合多条明细行，共享审批状态。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 北海
 * @version 1.0.0
 */
@Slf4j
@Service
public class StocktakeServiceImpl
        extends ServiceImpl<StocktakeMapper, WkfSupplyRequest>
        implements IStocktakeService {

    private static final String TYPE_ENUM_INVENTORY_CHECK = "INVENTORY_CHECK";
    private static final String TYPE_ENUM_PHARMACY_STOCKTAKE = "PHARMACY_STOCKTAKE";
    private static final String TYPE_ENUM_CABINET_STOCKTAKE = "CABINET_STOCKTAKE";
    private static final List<String> STOCKTAKE_TYPE_ENUMS = List.of(
            TYPE_ENUM_INVENTORY_CHECK,
            TYPE_ENUM_PHARMACY_STOCKTAKE,
            TYPE_ENUM_CABINET_STOCKTAKE
    );

    private static final String STATUS_DRAFT = "DRAFT";
    private static final String STATUS_PENDING_APPROVAL = "PENDING_APPROVAL";
    private static final String STATUS_APPROVAL = "APPROVAL";
    private static final String CATEGORY_ENUM_INVENTORY = "INVENTORY";
    private static final String PURPOSE_TYPE_PHARMACY = "PHARMACY";
    private static final String DEFAULT_TENANT_ID = "1";
    private static final String DEFAULT_ITEM_TABLE = "med_medication_definition";

    private static final DateTimeFormatter DATE_TIME_FORMATTER =
            DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
    private static final DateTimeFormatter BUS_NO_FORMATTER =
            DateTimeFormatter.ofPattern("yyyyMMddHHmmssSSS");

    private static final List<String> DELETABLE_STATUSES = List.of(STATUS_DRAFT, STATUS_PENDING_APPROVAL);

    @Resource
    private StocktakeMapper stocktakeMapper;

    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;

    @Resource
    private InventoryLockMapper inventoryLockMapper;

    @Resource
    private UserHolder userHolder;

    @Resource
    private IWkfSupplyRequestService wkfSupplyRequestService;

    @Override
    public StocktakeSaveVO addStocktake() {
        LocalDateTime now = LocalDateTime.now();
        return buildSaveVO(generateBusNo(now), STATUS_DRAFT, 0, BigDecimal.ZERO, now);
    }

    /**
     * 分页查询盘点单列表
     *
     * @param query 查询条件
     * @return 分页数据
     */
    @Override
    public PageDTO<StocktakeListVO> queryStocktakeList(StocktakeQuery query) {
        StocktakeQuery safeQuery = query == null ? new StocktakeQuery() : query;
        Page<StocktakeListVO> page = new Page<>(
                normalizePageIndex(safeQuery.getPageIndex()),
                normalizePageSize(safeQuery.getPageSize()));

        String startTimeStr = safeQuery.getStartTime() == null ? null
                : safeQuery.getStartTime().format(DATE_TIME_FORMATTER);
        String endTimeStr = safeQuery.getEndTime() == null ? null
                : safeQuery.getEndTime().format(DATE_TIME_FORMATTER);

        IPage<StocktakeListVO> result = stocktakeMapper.selectStocktakePage(
                page,
                safeQuery.getBusNo(),
                safeQuery.getStatusEnum(),
                safeQuery.getWarehouseId(),
                safeQuery.getWarehouseName(),
                safeQuery.getApplicantId(),
                startTimeStr,
                endTimeStr
        );

        PageDTO<StocktakeListVO> pageDTO = new PageDTO<>();
        pageDTO.setPageIndex(result.getCurrent());
        pageDTO.setPageSize(result.getSize());
        pageDTO.setTotal(result.getTotal());
        pageDTO.setPages(result.getPages());
        pageDTO.setRows(result.getRecords());
        return pageDTO;
    }

    /**
     * 查询盘点单详情（含明细列表）
     *
     * @param id 盘点单号（bus_no）或明细行 ID
     * @return 盘点单详情
     */
    @Override
    public StocktakeDetailVO queryStocktakeDetail(String id) {
        if (!StringUtils.hasText(id)) {
            throw new IllegalArgumentException("盘点单号不能为空");
        }
        StocktakeDetailVO detailVO = stocktakeMapper.selectStocktakeDetail(id.trim());
        if (detailVO == null) {
            throw new IllegalArgumentException("盘点单不存在：" + id);
        }

        List<StocktakeItemVO> items = stocktakeMapper.selectStocktakeItems(detailVO.getBusNo());
        detailVO.setItems(items == null ? Collections.emptyList() : items);
        return detailVO;
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeSaveVO saveStocktake(StocktakeSaveDTO dto) {
        return persistStocktake(dto, STATUS_DRAFT);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public StocktakeSaveVO saveAndSubmitStocktake(StocktakeSaveDTO dto) {
        // 先以草稿落库，再走完整提交审批流程（状态流转 + 锁库存 + 冻结快照），
        // 避免直接插入 PENDING_APPROVAL 导致与 /stocktake-approval/submit 校验冲突
        StocktakeSaveVO draftVO = persistStocktake(dto, STATUS_DRAFT);

        OperatorDTO operator = new OperatorDTO();
        operator.setBusNo(draftVO.getBusNo());
        UserDTO currentUser = getCurrentUserQuietly();
        operator.setOperatorId(firstText(
                currentUser == null ? null : currentUser.getId(),
                dto.getApplicantId(),
                "system"));
        wkfSupplyRequestService.submit(operator);

        return buildSaveVO(draftVO.getBusNo(), STATUS_APPROVAL,
                draftVO.getItemCount(), draftVO.getTotalPrice(), LocalDateTime.now(), dto.getWarehouseId());
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public SupplyDocumentDeleteVO deleteStocktake(StocktakeDeleteDTO dto) {
        if (dto == null || !StringUtils.hasText(dto.getBusNo())) {
            throw new IllegalArgumentException("盘点单号不能为空");
        }
        String busNo = dto.getBusNo().trim();

        WkfSupplyRequest record = stocktakeMapper.selectOneByBusNo(busNo);
        if (record == null) {
            throw new IllegalArgumentException("盘点单不存在：" + busNo);
        }
        if (!isStocktakeType(record.getTypeEnum())) {
            throw new IllegalArgumentException("单据类型非盘点单：" + busNo);
        }
        if (!DELETABLE_STATUSES.contains(record.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "只有草稿或待审核状态的盘点单可以删除，当前状态：" + record.getStatusEnum());
        }

        int unlockedLocks = 0;
        try {
            inventoryLockMapper.deleteByBusNo(busNo);
            unlockedLocks = 1;
        } catch (Exception e) {
            log.warn("删除库存锁定记录失败：busNo={}, error={}", busNo, e.getMessage());
        }

        UserDTO currentUser = getCurrentUserQuietly();
        String deletedBy = currentUser != null && currentUser.getId() != null
                ? currentUser.getId() : "system";
        LocalDateTime now = LocalDateTime.now();

        int deletedLines = stocktakeMapper.softDeleteByBusNo(
                busNo, deletedBy, now.format(DATE_TIME_FORMATTER));
        if (deletedLines == 0) {
            throw new IllegalArgumentException("删除盘点单失败，单据可能已被其他操作修改：" + busNo);
        }

        log.info("盘点单 {} 删除成功，删除 {} 行，解锁 {} 条锁定记录", busNo, deletedLines, unlockedLocks);

        SupplyDocumentDeleteVO vo = new SupplyDocumentDeleteVO();
        vo.setBusNo(busNo);
        vo.setDeletedLines(deletedLines);
        vo.setUnlockedLocks(unlockedLocks);
        return vo;
    }

    private StocktakeSaveVO persistStocktake(StocktakeSaveDTO dto, String targetStatus) {
        validateSaveDTO(dto);

        LocalDateTime now = LocalDateTime.now();
        String busNo = StringUtils.hasText(dto.getBusNo()) ? dto.getBusNo().trim() : generateBusNo(now);
        WkfSupplyRequest existingRecord = stocktakeMapper.selectOneByBusNo(busNo);
        if (existingRecord != null && !STATUS_DRAFT.equals(existingRecord.getStatusEnum())) {
            throw new IllegalArgumentException(
                    "只有草稿状态的盘点单可以保存，当前状态：" + existingRecord.getStatusEnum());
        }

        String typeEnum = existingRecord != null && isStocktakeType(existingRecord.getTypeEnum())
                ? existingRecord.getTypeEnum()
                : TYPE_ENUM_INVENTORY_CHECK;

        // 库房标识统一归一为机构 ID（兼容前端传库房名称），一次解析全明细复用
        String warehouseId = resolveWarehouseId(dto.getWarehouseId());

        // 盘点单编辑保存采用全量替换：同一 bus_no 下旧明细先逻辑删除，再插入最新明细。
        stocktakeMapper.delete(new LambdaQueryWrapper<WkfSupplyRequest>()
                .eq(WkfSupplyRequest::getBusNo, busNo));

        UserDTO currentUser = getCurrentUserQuietly();
        String applicantId = firstText(
                currentUser == null ? null : currentUser.getId(),
                dto.getApplicantId(),
                "system"
        );
        String operatorId = applicantId;
        String tenantId = firstText(dto.getTenantId(), DEFAULT_TENANT_ID);

        BigDecimal totalPrice = BigDecimal.ZERO;
        int itemCount = 0;
        for (int i = 0; i < dto.getItems().size(); i++) {
            StocktakeItemSaveDTO item = dto.getItems().get(i);
            WkfSupplyRequest record = buildStocktakeRecord(
                    dto,
                    item,
                    busNo,
                    typeEnum,
                    targetStatus,
                    tenantId,
                    applicantId,
                    operatorId,
                    now,
                    warehouseId,
                    i + 1
            );
            int rows = stocktakeMapper.insert(record);
            if (rows <= 0) {
                throw new IllegalArgumentException("保存盘点单明细失败：" + item.getItemId());
            }
            totalPrice = addIfNotNull(totalPrice, record.getTotalPrice());
            itemCount++;
        }

        return buildSaveVO(busNo, targetStatus, itemCount, totalPrice, now, dto.getWarehouseId());
    }

    private WkfSupplyRequest buildStocktakeRecord(StocktakeSaveDTO dto,
                                                 StocktakeItemSaveDTO item,
                                                 String busNo,
                                                 String typeEnum,
                                                 String statusEnum,
                                                 String tenantId,
                                                 String applicantId,
                                                 String operatorId,
                                                 LocalDateTime now,
                                                 String warehouseId,
                                                 int rowNo) {
        BigDecimal itemQuantity = resolveItemQuantity(item);
        BigDecimal totalPrice = resolveTotalPrice(item);
        BigDecimal profitLossPrice = resolveProfitLossPrice(item, itemQuantity);

        WkfSupplyRequest record = new WkfSupplyRequest();
        record.setTenantId(tenantId);
        record.setCreatedAt(now);
        record.setUpdatedAt(now);
        record.setCreatedBy(operatorId);
        record.setUpdatedBy(operatorId);
        record.setIsDeleted(false);
        record.setVersion(0);
        record.setBusNo(busNo);
        record.setTypeEnum(typeEnum);
        record.setStatusEnum(statusEnum);
        record.setCategoryEnum(CATEGORY_ENUM_INVENTORY);
        record.setItemTable(DEFAULT_ITEM_TABLE);
        record.setItemQuantity(itemQuantity);
        record.setTotalQuantity(item.getTotalQuantity());
        record.setItemId(item.getItemId().trim());
        record.setItemName(trimToNull(item.getItemName()));
        record.setSpecification(trimToNull(item.getSpecification()));
        record.setManufacturer(trimToNull(item.getManufacturer()));
        record.setUnitCode(resolveUnitCode(item, warehouseId, rowNo));
        record.setOccurrenceTime(dto.getOccurrenceTime() == null ? now : dto.getOccurrenceTime());
        record.setPurposeTypeEnum(firstText(dto.getPurposeTypeEnum(), PURPOSE_TYPE_PHARMACY));
        record.setPurposeLocationId(warehouseId);
        record.setApplicantId(applicantId);
        record.setApplyTime(STATUS_PENDING_APPROVAL.equals(statusEnum) ? now : null);
        record.setLotNumber(trimToNull(item.getLotNumber()));
        record.setTraceNo(trimToNull(item.getTraceNo()));
        record.setPrice(item.getPrice());
        record.setTotalPrice(totalPrice);
        record.setProfitLossPrice(profitLossPrice);
        record.setRemake(firstText(item.getRemark(), dto.getRemark()));
        return record;
    }

    private void validateSaveDTO(StocktakeSaveDTO dto) {
        if (dto == null) {
            throw new IllegalArgumentException("盘点单保存参数不能为空");
        }
        if (!StringUtils.hasText(dto.getWarehouseId())) {
            throw new IllegalArgumentException("库房标识不能为空");
        }
        if (dto.getItems() == null || dto.getItems().isEmpty()) {
            throw new IllegalArgumentException("盘点明细不能为空");
        }
        for (int i = 0; i < dto.getItems().size(); i++) {
            validateSaveItem(dto.getItems().get(i), i + 1);
        }
    }

    private void validateSaveItem(StocktakeItemSaveDTO item, int rowNo) {
        if (item == null) {
            throw new IllegalArgumentException("第" + rowNo + "行盘点明细不能为空");
        }
        if (!StringUtils.hasText(item.getItemId())) {
            throw new IllegalArgumentException("第" + rowNo + "行物品标识不能为空");
        }
        if (item.getTotalQuantity() == null) {
            throw new IllegalArgumentException("第" + rowNo + "行实盘数量不能为空");
        }
        if (item.getItemQuantity() == null && item.getBookQuantity() == null) {
            throw new IllegalArgumentException("第" + rowNo + "行盈亏数量不能为空，或传入账面数量用于计算");
        }
    }

    /**
     * 解析单位编码：前端传了直接用；未传时依次从
     * 库存记录（同库房+药品+批号）→ 药品定义库存单位 自动回填，
     * 与审批环节定位库存的口径保持一致
     */
    private String resolveUnitCode(StocktakeItemSaveDTO item, String warehouseId, int rowNo) {
        if (StringUtils.hasText(item.getUnitCode())) {
            return item.getUnitCode().trim();
        }
        if (StringUtils.hasText(item.getLotNumber())) {
            WkfInventoryItem inventoryItem = wkfInventoryItemMapper.findByItemLotAndLocation(
                    item.getItemId().trim(), item.getLotNumber().trim(), warehouseId);
            if (inventoryItem != null && StringUtils.hasText(inventoryItem.getUnitCode())) {
                return inventoryItem.getUnitCode();
            }
        }
        String stockUnitCode = stocktakeMapper.findStockUnitCodeByItemId(item.getItemId().trim());
        if (StringUtils.hasText(stockUnitCode)) {
            return stockUnitCode;
        }
        throw new IllegalArgumentException("第" + rowNo + "行单位编码不能为空，且无法自动回填："
                + item.getItemId());
    }

    private BigDecimal resolveItemQuantity(StocktakeItemSaveDTO item) {
        if (item.getItemQuantity() != null) {
            return item.getItemQuantity();
        }
        return item.getTotalQuantity().subtract(item.getBookQuantity());
    }

    private BigDecimal resolveTotalPrice(StocktakeItemSaveDTO item) {
        if (item.getTotalPrice() != null) {
            return item.getTotalPrice();
        }
        if (item.getPrice() == null) {
            return null;
        }
        return item.getPrice().multiply(item.getTotalQuantity());
    }

    private BigDecimal resolveProfitLossPrice(StocktakeItemSaveDTO item, BigDecimal itemQuantity) {
        if (item.getProfitLossPrice() != null) {
            return item.getProfitLossPrice();
        }
        if (item.getPrice() == null) {
            return null;
        }
        return item.getPrice().multiply(itemQuantity);
    }

    private BigDecimal addIfNotNull(BigDecimal total, BigDecimal value) {
        return value == null ? total : total.add(value);
    }

    private boolean isStocktakeType(String typeEnum) {
        return STOCKTAKE_TYPE_ENUMS.contains(typeEnum);
    }

    /**
     * 将前端传入的库房标识（可能是机构 ID，也可能是库房名称）归一为 adm_organization 的 ID，
     * 保证 purpose_location_id 与库存表 location_id 口径一致；无法识别时原样保留并记录告警
     */
    private String resolveWarehouseId(String warehouseId) {
        String trimmed = warehouseId == null ? null : warehouseId.trim();
        if (!StringUtils.hasText(trimmed)) {
            return trimmed;
        }
        String resolved = stocktakeMapper.resolveLocationId(trimmed);
        if (resolved == null) {
            log.warn("库房标识无法解析为机构ID，按原值保存: {}", trimmed);
            return trimmed;
        }
        return resolved;
    }

    private StocktakeSaveVO buildSaveVO(String busNo,
                                        String statusEnum,
                                        int itemCount,
                                        BigDecimal totalPrice,
                                        LocalDateTime operateTime) {
        return buildSaveVO(busNo, statusEnum, itemCount, totalPrice, operateTime, null);
    }

    private StocktakeSaveVO buildSaveVO(String busNo,
                                        String statusEnum,
                                        int itemCount,
                                        BigDecimal totalPrice,
                                        LocalDateTime operateTime,
                                        String warehouseId) {
        StocktakeSaveVO vo = new StocktakeSaveVO();
        vo.setId(busNo);
        vo.setBusNo(busNo);
        vo.setStatusEnum(statusEnum);
        vo.setCreatedAt(operateTime);
        vo.setWarehouseId(warehouseId);
        vo.setItemCount(itemCount);
        vo.setTotalPrice(totalPrice);
        vo.setOperateTime(operateTime);
        vo.setItems(Collections.emptyList());

        StocktakeDetailVO detailVO = stocktakeMapper.selectStocktakeDetail(busNo);
        if (detailVO != null) {
            vo.setId(detailVO.getId());
            vo.setCreatedBy(detailVO.getCreatedBy());
            vo.setCreatedByName(detailVO.getCreatedByName());
            vo.setCreatedAt(detailVO.getCreatedAt());
            vo.setApproverId(detailVO.getApproverId());
            vo.setApproverName(detailVO.getApproverName());
            vo.setApprovalTime(detailVO.getApprovalTime());
            vo.setWarehouseId(detailVO.getWarehouseId());
            vo.setWarehouseName(detailVO.getWarehouseName());
            vo.setStocktakePharmacy(detailVO.getStocktakePharmacy());
            vo.setTotalPrice(detailVO.getTotalPrice());
            vo.setItemCount(detailVO.getItemCount());
            List<StocktakeItemVO> items = stocktakeMapper.selectStocktakeItems(busNo);
            vo.setItems(items == null ? Collections.emptyList() : items);
        }
        return vo;
    }

    private String generateBusNo(LocalDateTime now) {
        return "PST" + now.format(BUS_NO_FORMATTER);
    }

    private UserDTO getCurrentUserQuietly() {
        try {
            return userHolder.getCurrentUser();
        } catch (Exception e) {
            log.debug("获取当前用户失败，使用默认操作人：{}", e.getMessage());
            return null;
        }
    }

    private String firstText(String... values) {
        if (values == null) {
            return null;
        }
        for (String value : values) {
            if (StringUtils.hasText(value)) {
                return value.trim();
            }
        }
        return null;
    }

    private String trimToNull(String value) {
        return StringUtils.hasText(value) ? value.trim() : null;
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
