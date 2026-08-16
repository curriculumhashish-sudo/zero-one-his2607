package com.zeroone.star.pharmacy.service.impl;

import com.baomidou.mybatisplus.core.conditions.query.LambdaQueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.entity.WkfInventoryItem;
import com.zeroone.star.pharmacy.entity.WkfSupplyDelivery;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfInventoryItemMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyDeliveryMapper;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IInpatientPharmacyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseRejectDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
import com.zeroone.star.project.vo.j7.pharmacy.SummaryVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：住院药房服务实现类
 * </p>
 * <p>
 * 功能说明：实现住院发药驳回和住院退药确认两大业务功能。
 * 涉及的表包括：
 * - wkf_supply_request（供应请求表）：发药汇总单、退药主单据
 * - wkf_supply_delivery（供应交付表）：发药明细、退药明细
 * - wkf_inventory_item（库存项表）：药品库存回补
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class InpatientPharmacyServiceImpl implements IInpatientPharmacyService {

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private WkfSupplyDeliveryMapper wkfSupplyDeliveryMapper;

    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;

    // ==================== 住院发药驳回 ====================

    /**
     * 获取汇总单队列
     * <p>
     * 查询 wkf_supply_request 表中发药类型、状态为"待处理"的记录，
     * 按 bus_no 聚合为汇总单，按创建时间倒序排列。
     * </p>
     *
     * @return 汇总单视图对象列表
     */
    @Override
    public List<SummaryVO> getSummaryList() {
        // 查询 wkf_supply_request 表：发药类型 + 待处理状态，按创建时间倒序
        List<WkfSupplyRequest> list = wkfSupplyRequestMapper.selectList(
                new LambdaQueryWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getTypeEnum, "DISPENSE")
                        .eq(WkfSupplyRequest::getStatusEnum, "PENDING")
                        .orderByDesc(WkfSupplyRequest::getCreatedAt)
        );
        // 按 bus_no 去重聚合为汇总单
        return list.stream()
                .collect(Collectors.toMap(
                        WkfSupplyRequest::getBusNo,
                        item -> item,
                        (existing, replacement) -> existing
                ))
                .values().stream().map(item -> {
                    SummaryVO vo = new SummaryVO();
                    vo.setSummaryId(item.getBusNo());
                    vo.setSummaryCode(item.getBusNo());
                    vo.setSummaryName(item.getBusNo());
                    vo.setStatus(0);
                    vo.setCreateTime(item.getCreatedAt());
                    vo.setCreateBy(item.getCreatedBy());
                    return vo;
                }).collect(Collectors.toList());
    }

    /**
     * 根据汇总单ID获取患者药品项目
     * <p>
     * 汇总单ID对应 wkf_supply_request.bus_no。
     * 先查询该汇总单下的所有供应请求，再根据请求ID列表查询交付明细。
     * </p>
     *
     * @param summaryId 汇总单ID（对应 bus_no）
     * @return 药品项目视图对象列表
     */
    @Override
    public List<DrugItemVO> getDrugItemsBySummary(String summaryId) {
        // 第一步：根据 bus_no 查询 wkf_supply_request 表获取所有请求
        List<WkfSupplyRequest> requests = wkfSupplyRequestMapper.selectList(
                new LambdaQueryWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getBusNo, summaryId)
        );
        if (requests.isEmpty()) {
            return Collections.emptyList();
        }
        // 第二步：收集所有请求ID
        List<String> requestIds = requests.stream()
                .map(WkfSupplyRequest::getId)
                .collect(Collectors.toList());
        // 第三步：根据请求ID列表查询 wkf_supply_delivery 表获取交付明细
        List<WkfSupplyDelivery> deliveries = wkfSupplyDeliveryMapper.selectList(
                new LambdaQueryWrapper<WkfSupplyDelivery>()
                        .in(WkfSupplyDelivery::getRequestId, requestIds)
        );
        return deliveries.stream().map(item -> {
            DrugItemVO vo = new DrugItemVO();
            vo.setItemId(item.getId());
            vo.setDrugCode(item.getItemId());
            vo.setQuantity(item.getQuantity() != null ? item.getQuantity().intValue() : null);
            vo.setUnit(item.getUnitCode());
            vo.setBatchNo(item.getLotNumber());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 执行发药核对（发药后截单）
     * <p>
     * 更新 wkf_supply_request 表中指定请求的状态为"已完成"。
     * </p>
     *
     * @param dispensingId 发药请求ID
     */
    @Override
    @Transactional
    public void confirmDispensing(String dispensingId) {
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dispensingId)
                        .set(WkfSupplyRequest::getStatusEnum, "COMPLETED")
        );
    }

    /**
     * 驳回患者发药单据，同步驳回汇总单
     * <p>
     * 两步操作在同一事务中执行：
     * 1. 更新指定请求的状态为"已驳回"，记录驳回原因
     * 2. 同步驳回同一汇总单（bus_no）下的所有请求
     * </p>
     *
     * @param dto 发药驳回参数
     */
    @Override
    @Transactional
    public void rejectDispensing(DispenseRejectDTO dto) {
        // 1. 更新指定请求：状态=已驳回，记录驳回原因
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getDispensingId())
                        .set(WkfSupplyRequest::getStatusEnum, "REJECTED")
                        .set(WkfSupplyRequest::getReason, dto.getRejectReason())
        );
        // 2. 同步驳回同一汇总单下的其他待处理请求
        if (dto.getSummaryId() != null) {
            wkfSupplyRequestMapper.update(null,
                    new LambdaUpdateWrapper<WkfSupplyRequest>()
                            .eq(WkfSupplyRequest::getBusNo, dto.getSummaryId())
                            .eq(WkfSupplyRequest::getStatusEnum, "PENDING")
                            .set(WkfSupplyRequest::getStatusEnum, "REJECTED")
            );
        }
    }

    // ==================== 住院退药确认 ====================

    /**
     * 分页查询退药患者队列
     * <p>
     * 查询 wkf_supply_request 表中退药类型为"住院退药"、状态为"待确认"的记录，
     * 按创建时间倒序排列，分页返回。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    @Override
    public PageDTO<PatientQueueVO> queryReturnPatientQueue(PatientQueueQuery query) {
        Page<WkfSupplyRequest> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfSupplyRequest> wrapper = new LambdaQueryWrapper<WkfSupplyRequest>()
                .eq(WkfSupplyRequest::getTypeEnum, "RETURN_INPATIENT")
                .eq(WkfSupplyRequest::getStatusEnum, "PENDING")
                .orderByDesc(WkfSupplyRequest::getCreatedAt);
        Page<WkfSupplyRequest> result = wkfSupplyRequestMapper.selectPage(page, wrapper);
        // 手动映射到 PatientQueueVO
        Page<PatientQueueVO> voPage = new Page<>(result.getCurrent(), result.getSize(), result.getTotal());
        voPage.setRecords(result.getRecords().stream().map(item -> {
            PatientQueueVO vo = new PatientQueueVO();
            vo.setPatientId(item.getPatientId());
            vo.setStatus(0);
            return vo;
        }).collect(Collectors.toList()));
        return PageDTO.create(voPage, vo -> vo);
    }

    /**
     * 获取患者退药明细信息
     * <p>
     * 查询 wkf_supply_delivery 表中指定供应请求ID的所有交付明细。
     * </p>
     *
     * @param returnId 退药请求ID（对应 wkf_supply_delivery.request_id）
     * @return 药品明细列表
     */
    @Override
    public List<DrugItemVO> getReturnDrugItems(String returnId) {
        List<WkfSupplyDelivery> deliveries = wkfSupplyDeliveryMapper.selectList(
                new LambdaQueryWrapper<WkfSupplyDelivery>()
                        .eq(WkfSupplyDelivery::getRequestId, returnId)
        );
        if (deliveries.isEmpty()) {
            return Collections.emptyList();
        }
        return deliveries.stream().map(item -> {
            DrugItemVO vo = new DrugItemVO();
            vo.setItemId(item.getId());
            vo.setDrugCode(item.getItemId());
            vo.setQuantity(item.getQuantity() != null ? item.getQuantity().intValue() : null);
            vo.setUnit(item.getUnitCode());
            vo.setBatchNo(item.getLotNumber());
            return vo;
        }).collect(Collectors.toList());
    }

    /**
     * 确认退药（对勾选药品执行退药，同步回补药品库存）
     * <p>
     * 两步操作在同一事务中执行：
     * 1. 更新供应请求状态为"已确认"，记录审批人和审批时间
     * 2. 对勾选的药品回补库存：查询 wkf_inventory_item 找到对应库存记录，
     *    将库存数量增加退药数量
     * </p>
     *
     * @param dto 退药确认参数
     */
    @Override
    @Transactional
    public void confirmReturn(ReturnConfirmDTO dto) {
        // 1. 更新供应请求：状态=已确认，记录审批信息
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getReturnId())
                        .set(WkfSupplyRequest::getStatusEnum, "CONFIRMED")
                        .set(WkfSupplyRequest::getApproverId, dto.getOperator())
                        .set(WkfSupplyRequest::getApprovalTime, LocalDateTime.now())
        );

        // 2. 对勾选的药品回补库存
        if (dto.getItemIds() != null && !dto.getItemIds().isEmpty()) {
            List<WkfSupplyDelivery> returnItems = wkfSupplyDeliveryMapper.selectList(
                    new LambdaQueryWrapper<WkfSupplyDelivery>()
                            .in(WkfSupplyDelivery::getId, dto.getItemIds())
            );
            for (WkfSupplyDelivery item : returnItems) {
                // 查询库存：根据物品表名和物品标识查找库存记录
                WkfInventoryItem stock = wkfInventoryItemMapper.selectOne(
                        new LambdaQueryWrapper<WkfInventoryItem>()
                                .eq(WkfInventoryItem::getItemTable, item.getItemTable())
                                .eq(WkfInventoryItem::getItemId, item.getItemId())
                );
                if (stock != null) {
                    BigDecimal newQty = (stock.getQuantity() != null ? stock.getQuantity() : BigDecimal.ZERO)
                            .add(item.getQuantity() != null ? item.getQuantity() : BigDecimal.ZERO);
                    wkfInventoryItemMapper.update(null,
                            new LambdaUpdateWrapper<WkfInventoryItem>()
                                    .eq(WkfInventoryItem::getId, stock.getId())
                                    .set(WkfInventoryItem::getQuantity, newQty)
                    );
                }
            }
        }
    }
}
