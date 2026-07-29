package com.zeroone.star.pharmacy.service.impl;


import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.core.metadata.OrderItem;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.*;
import com.zeroone.star.pharmacy.mapper.*;
import com.zeroone.star.pharmacy.service.IWkfSupplyRequestService;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.project.query.j6.pharmacy.PharmacyInventoryDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.stocktake.PharmacyInventoryQuery;
import com.zeroone.star.project.vo.j6.pharmacy.*;
import com.zeroone.star.project.vo.j6.pharmacy.stocktake.PharmacyInventoryVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.CollectionUtils;
import org.springframework.util.StringUtils;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
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
public class WkfSupplyRequestServiceImpl extends ServiceImpl<WkfSupplyRequestMapper, WkfSupplyRequest> implements IWkfSupplyRequestService {

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
        queryWrapper.eq(WkfSupplyRequest::getIsDeleted, false);
        WkfSupplyRequest stocktake = this.getOne(queryWrapper);
        // 判断单据是否存在
        if (stocktake == null) {
            throw new RuntimeException("单据不存在或已经被删除");
        }
        // 2. 业务规则校验：只有PENDING_APPROVAL状态允许删除
        if (!"PENDING_APPROVAL".equals(stocktake.getStatusEnum())) {
            throw new RuntimeException("单据已提交审批，无法执行删除操作！");
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
        // 1、获取前端传入业务单据号
        String stocktakeNo = query.getStocktakeNo();
        if (!StringUtils.hasText(stocktakeNo)) {
            throw new RuntimeException("盘点单据号不能为空");
        }
        // 根据单据号busNo查询主单
        LambdaQueryWrapper<WkfSupplyRequest> reqWrapper = new LambdaQueryWrapper<>();
        reqWrapper.eq(WkfSupplyRequest::getBusNo, stocktakeNo);
        reqWrapper.eq(WkfSupplyRequest::getIsDeleted, false);
        WkfSupplyRequest mainRequest = baseMapper.selectOne(reqWrapper);
        if (mainRequest == null) {
            throw new RuntimeException("盘点单据不存在或已删除");
        }
        // 2、查询库房名称
        String locationName = null;
        if (StringUtils.hasText(mainRequest.getPurposeLocationId())) {
            AdmLocation location = admLocationMapper.selectById(mainRequest.getPurposeLocationId());
            if (location != null) {
                locationName = location.getName();
            }
        }
        // 3、查询单据明细
        LambdaQueryWrapper<WkfSupplyDelivery> deliveryWrapper = new LambdaQueryWrapper<>();
        deliveryWrapper.eq(WkfSupplyDelivery::getRequestId, mainRequest.getId());
        deliveryWrapper.eq(WkfSupplyDelivery::getIsDeleted, false);
        List<WkfSupplyDelivery> deliveryList = wkfSupplyDeliveryMapper.selectList(deliveryWrapper);
        // 收集所有药品ID，批量查询药品基础信息
        List<String> itemIdList = deliveryList.stream()
                .map(WkfSupplyDelivery::getItemId)
                .filter(StringUtils::hasText)
                .distinct()
                .collect(Collectors.toList());
        Map<String, MedMedicationDefinition> medMap = new HashMap<>();
        if (!CollectionUtils.isEmpty(itemIdList)) {
            List<MedMedicationDefinition> medList = medMedicationDefinitionMapper.selectBatchIds(itemIdList);
            medMap = medList.stream()
                    .collect(Collectors.toMap(MedMedicationDefinition::getId, m -> m, (v1, v2) -> v1));
        }
        // 批量查询库存信息
        Map<String, WkfInventoryItem> inventoryItemMap = new HashMap<>();
        if (!CollectionUtils.isEmpty(itemIdList) && StringUtils.hasText(mainRequest.getPurposeLocationId())) {
            LambdaQueryWrapper<WkfInventoryItem> invWrapper = new LambdaQueryWrapper<>();
            invWrapper.in(WkfInventoryItem::getItemId, itemIdList);
            invWrapper.eq(WkfInventoryItem::getLocationId, mainRequest.getPurposeLocationId());
            invWrapper.eq(WkfInventoryItem::getIsDeleted, false);
            List<WkfInventoryItem> inventoryItemList = wkfInventoryItemMapper.selectList(invWrapper);
            inventoryItemMap = inventoryItemList.stream()
                    .collect(Collectors.toMap(WkfInventoryItem::getItemId, item -> item, (v1, v2) -> v1));
        }
        // 批量查询药品收费单价
        Map<String, BigDecimal> itemPriceMap = new HashMap<>();
        if (!CollectionUtils.isEmpty(itemIdList)) {
            LambdaQueryWrapper<AdmChargeItemDefinition> chargeWrapper = new LambdaQueryWrapper<>();
            chargeWrapper.eq(AdmChargeItemDefinition::getInstanceTable, "med_medication_definition")
                    .in(AdmChargeItemDefinition::getInstanceId, itemIdList)
                    .eq(AdmChargeItemDefinition::getStatusEnum, "ACTIVE")
                    .orderByAsc(AdmChargeItemDefinition::getPriority);
            List<AdmChargeItemDefinition> chargeList = admChargeItemDefinitionMapper.selectList(chargeWrapper);
            // 同一药品只保留优先级最高一条价格
            for (AdmChargeItemDefinition charge : chargeList) {
                if (!itemPriceMap.containsKey(charge.getInstanceId())) {
                    itemPriceMap.put(charge.getInstanceId(), charge.getPriceValue());
                }
            }
        }
        // 4、组装外层VO
        PharmacyInventoryDetailVO detailVO = new PharmacyInventoryDetailVO();
        detailVO.setBusNo(mainRequest.getBusNo());
        detailVO.setStatusEnum(mainRequest.getStatusEnum());
        detailVO.setPurposeLocationId(mainRequest.getPurposeLocationId());
        detailVO.setPurposeLocationName(locationName);
        detailVO.setApplicantId(mainRequest.getApplicantId());
        detailVO.setPractitionerId(mainRequest.getPractitionerId());
        detailVO.setApplyTime(mainRequest.getApplyTime() != null ? mainRequest.getApplyTime().format(FORMATTER) : null);
        detailVO.setApproverId(mainRequest.getApproverId());
        detailVO.setApprovalTime(mainRequest.getApprovalTime() != null ? mainRequest.getApprovalTime().format(FORMATTER) : null);
        List<PharmacyInventoryItemVO> lineVoList = new ArrayList<>();
        if (!CollectionUtils.isEmpty(deliveryList)) {
            for (WkfSupplyDelivery delivery : deliveryList) {
                PharmacyInventoryItemVO itemVO = new PharmacyInventoryItemVO();
                // 明细基础主键
                itemVO.setLineId(delivery.getId());
                itemVO.setItemTable(delivery.getItemTable());
                itemVO.setItemId(delivery.getItemId());
                itemVO.setLotNumber(delivery.getLotNumber());
                itemVO.setUnitCode(delivery.getUnitCode());
                itemVO.setOccurrenceTime(delivery.getOccurrenceTime() != null ? delivery.getOccurrenceTime().format(FORMATTER) : null);

                // 实盘数量
                BigDecimal actualQuantity = Optional.ofNullable(delivery.getQuantity()).orElse(BigDecimal.ZERO);
                itemVO.setActualQuantity(actualQuantity);

                // 账面库存 + 补充有效期
                BigDecimal bookQuantity = BigDecimal.ZERO;
                WkfInventoryItem inventoryItem = inventoryItemMap.get(delivery.getItemId());
                if (inventoryItem != null) {
                    bookQuantity = Optional.ofNullable(inventoryItem.getQuantity()).orElse(BigDecimal.ZERO);
                    // 从库存带出效期
                    if (inventoryItem.getExpirationDate() != null) {
                        itemVO.setExpirationDate(inventoryItem.getExpirationDate().format(FORMATTER));
                    }
                }
                itemVO.setBookQuantity(bookQuantity);
                // 获取药品基础信息
                MedMedicationDefinition med = medMap.get(delivery.getItemId());
                // 从收费项目取单价，无则0
                BigDecimal price = itemPriceMap.getOrDefault(delivery.getItemId(), BigDecimal.ZERO);
                ProductCatalogVO catalogVO = new ProductCatalogVO();
                if (med != null) {
                    itemVO.setProductName(med.getName());
                    // 规格：实体 totalVolume
                    itemVO.setSpecification(med.getTotalVolume());
                    // 生产厂商
                    itemVO.setManufacturer(med.getManufacturerText());
                    // 填充ProductCatalogVO
                    catalogVO.setMerchandiseName(med.getMerchandiseName());
                    catalogVO.setCategoryCode(med.getCategoryCode());
                    catalogVO.setProductBusNo(med.getBusNo());
                    catalogVO.setTotalVolume(med.getTotalVolume());
                    catalogVO.setMinUnitCode(med.getSplitUnitCode());
                    catalogVO.setDefaultPurchaseUnitCode(med.getPurchaseUnitCode());
                    catalogVO.setPurchaseUnitRatioToSplit(med.getPurchaseUnitRatio());
                }
                itemVO.setPrice(price);
                // 盈亏数量 = 实际数量 - 账面数量
                BigDecimal profitLossQuantity = actualQuantity.subtract(bookQuantity);
                itemVO.setProfitLossQuantity(profitLossQuantity);
                // 实际金额、盈亏金额计算
                BigDecimal actualAmount = actualQuantity.multiply(price).setScale(2, BigDecimal.ROUND_HALF_UP);
                itemVO.setActualAmount(actualAmount);
                BigDecimal profitLossAmount = profitLossQuantity.multiply(price).setScale(2, BigDecimal.ROUND_HALF_UP);
                itemVO.setProfitLossAmount(profitLossAmount);
                itemVO.setProductCatalog(catalogVO);
                lineVoList.add(itemVO);
            }
        }
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
            case "PENDING_APPROVAL" -> "待提交审批";
            case "APPROVAL" -> "审核中";
            case "AGREE" -> "同意";
            case "REJECT" -> "驳回";
            default -> "未知状态";
        };
    }


    @Override
    public PharmacyInventoryListVO listInventory(PharmacyInventoryQuery query) {
        //1.构建分页对象
        Page<WkfSupplyRequest> page = new Page<>();
        page.setCurrent(query.getPageIndex());
        page.setSize(query.getPageSize());
        // 2.构造主表查询条件
        var wrapper = new com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper<WkfSupplyRequest>();
        wrapper.eq(WkfSupplyRequest::getIsDeleted, false);
        wrapper.eq(WkfSupplyRequest::getTypeEnum, "CABINET_STOCKTAKE");
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
                PharmacyInventoryListVO resultVO = new PharmacyInventoryListVO();
                resultVO.setContent(new ArrayList<>());
                resultVO.setTotalCount(0L);
                resultVO.setPageIndex(query.getPageIndex());
                resultVO.setPageSize(query.getPageSize());
                return resultVO;
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

        // 4.批量统计明细条目数量 wkf_supply_delivery
        List<RequestSummaryDTO> summaryList = wkfSupplyDeliveryMapper.selectSummaryByRequestIds(requestIdList);
        Map<String, RequestSummaryDTO> summaryMap = summaryList.stream()
                .collect(Collectors.toMap(RequestSummaryDTO::getRequestId, s -> s, (v1, v2) -> v1));

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
            RequestSummaryDTO summary = summaryMap.get(req.getId());
            vo.setItemCount(summary != null ? summary.getItemCount() : 0);

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
