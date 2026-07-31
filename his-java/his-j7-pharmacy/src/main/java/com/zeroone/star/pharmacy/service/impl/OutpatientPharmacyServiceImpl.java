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
import com.zeroone.star.pharmacy.service.IOutpatientPharmacyService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnConfirmDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.pharmacy.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.pharmacy.DrugItemVO;
import com.zeroone.star.project.vo.j7.pharmacy.PatientQueueVO;
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
 * 描述：门诊药房服务实现类
 * </p>
 * <p>
 * 功能说明：实现门诊拒绝退药业务功能。
 * 涉及的表包括：
 * - wkf_supply_request（供应请求表）：退药主单据，退药类型为"门诊退药"
 * - wkf_supply_delivery（供应交付表）：退药明细
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class OutpatientPharmacyServiceImpl implements IOutpatientPharmacyService {

    @Resource
    private WkfSupplyRequestMapper wkfSupplyRequestMapper;

    @Resource
    private WkfSupplyDeliveryMapper wkfSupplyDeliveryMapper;

    @Resource
    private WkfInventoryItemMapper wkfInventoryItemMapper;

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 查询 wkf_supply_request 表中退药类型为"门诊退药"、状态为"待处理"的记录，
     * 按创建时间倒序排列，分页返回。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    @Override
    public PageDTO<PatientQueueVO> queryOutpatientReturnQueue(PatientQueueQuery query) {
        Page<WkfSupplyRequest> page = new Page<>(query.getPageIndex(), query.getPageSize());
        LambdaQueryWrapper<WkfSupplyRequest> wrapper = new LambdaQueryWrapper<WkfSupplyRequest>()
                .eq(WkfSupplyRequest::getTypeEnum, "RETURN_OUTPATIENT")
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
     * 执行确认退药（回补库存）
     * <p>
     * 根据勾选的药品明细ID列表，回补 wkf_inventory_item 表库存数量，
     * 并更新 wkf_supply_request 表状态为"已完成"。
     * 业务流程：
     * 1. 校验退药单据存在且状态为"待处理"
     * 2. 根据勾选的明细ID查询 wkf_supply_delivery 获取药品信息
     * 3. 根据药品ID、批号、库房查询 wkf_inventory_item 库存项
     * 4. 回补库存数量（增加 quantity）
     * 5. 更新 wkf_supply_request 状态为"已完成"
     * 6. 更新 wkf_supply_delivery 状态为"已完成"
     * </p>
     *
     * @param dto 退药确认参数
     */
    @Override
    @Transactional
    public void confirmReturn(ReturnConfirmDTO dto) {
        // 1. 查询退药请求
        WkfSupplyRequest request = wkfSupplyRequestMapper.selectById(dto.getReturnId());
        if (request == null) {
            throw new IllegalArgumentException("退药单据不存在");
        }
        if (!"RETURN_OUTPATIENT".equals(request.getTypeEnum())) {
            throw new IllegalArgumentException("单据类型错误");
        }
        if (!"PENDING".equals(request.getStatusEnum())) {
            throw new IllegalArgumentException("单据状态错误，只有待处理状态才能确认退药");
        }

        // 2. 根据勾选的明细ID查询交付记录
        List<WkfSupplyDelivery> deliveries = wkfSupplyDeliveryMapper.selectBatchIds(dto.getItemIds());
        if (deliveries.isEmpty()) {
            throw new IllegalArgumentException("退药明细为空");
        }

        // 3. 回补库存
        for (WkfSupplyDelivery delivery : deliveries) {
            WkfInventoryItem inventoryItem = wkfInventoryItemMapper.findByItemLotAndLocation(
                    delivery.getItemId(),
                    delivery.getLotNumber(),
                    request.getSourceLocationId() != null ? request.getSourceLocationId() : request.getPurposeLocationId()
            );

            if (inventoryItem != null) {
                // 回补库存数量
                BigDecimal delta = delivery.getQuantity() != null ? delivery.getQuantity() : BigDecimal.ZERO;
                int updated = wkfInventoryItemMapper.updateQuantity(
                        inventoryItem.getId(),
                        delta,
                        inventoryItem.getVersion(),
                        dto.getOperator()
                );
                if (updated == 0) {
                    throw new IllegalArgumentException("库存更新失败，请重试");
                }
            }
        }

        // 4. 更新 wkf_supply_request 状态为"已完成"
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getReturnId())
                        .set(WkfSupplyRequest::getStatusEnum, "COMPLETED")
                        .set(WkfSupplyRequest::getApprovalTime, LocalDateTime.now())
                        .set(WkfSupplyRequest::getUpdatedBy, dto.getOperator())
        );

        // 5. 更新 wkf_supply_delivery 状态为"已完成"
        wkfSupplyDeliveryMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyDelivery>()
                        .in(WkfSupplyDelivery::getId, dto.getItemIds())
                        .set(WkfSupplyDelivery::getStatusEnum, "COMPLETED")
                        .set(WkfSupplyDelivery::getReceiveTime, LocalDateTime.now())
        );
    }

    /**
     * 执行拒绝退药
     * <p>
     * 更新 wkf_supply_request 表：设置状态为"已拒绝"，记录拒绝原因。
     * 同时校验退药类型为"门诊退药"，确保不会误操作住院退药。
     * </p>
     *
     * @param dto 拒绝退药参数
     */
    @Override
    @Transactional
    public void rejectReturn(ReturnRejectDTO dto) {
        wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getReturnId())
                        .eq(WkfSupplyRequest::getTypeEnum, "RETURN_OUTPATIENT")
                        .set(WkfSupplyRequest::getStatusEnum, "REJECTED")
                        .set(WkfSupplyRequest::getReason, dto.getRejectReason())
        );
    }
}
