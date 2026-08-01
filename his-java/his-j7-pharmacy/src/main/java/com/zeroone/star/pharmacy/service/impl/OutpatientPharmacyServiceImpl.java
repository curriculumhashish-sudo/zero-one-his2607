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

import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

/**
 * <p>
 * 描述：门诊药房服务实现类
 * </p>
 * <p>
 * 功能说明：实现门诊退药业务功能，包括确认退药（回补库存）和拒绝退药。
 * 涉及的表包括：
 * - wkf_supply_request（供应请求表）：退药主单据，退药类型为"门诊退药"
 * - wkf_supply_delivery（供应交付表）：退药明细
 * - wkf_inventory_item（库存项表）：库存记录，确认退药时回补库存数量
 * </p>
 * <p>
 * 版权：&copy;01星球
 * </p>
 * <p>
 * 地址：01星球总部
 * </p>
 * 
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
                        .eq(WkfSupplyDelivery::getRequestId, returnId));
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
                        .set(WkfSupplyRequest::getReason, dto.getRejectReason()));
    }

    /**
     * 执行确认退药（回补库存）
     * <p>
     * 药师核对退药明细后点击确认退药，在一个事务内完成以下操作：
     * </p>
     * <ol>
     *   <li><b>校验退药主单据</b>：查询 wkf_supply_request，确认单据存在、类型为 RETURN_OUTPATIENT、状态为 PENDING</li>
     *   <li><b>校验退药明细</b>：批量查询 wkf_supply_delivery，确认勾选的明细属于该退药单</li>
     *   <li><b>回补库存</b>：遍历每条明细，按 item_id + lot_number + purpose_location_id 匹配
     *       wkf_inventory_item 库存记录，调用 updateQuantity（乐观锁）增加库存数量</li>
     *   <li><b>更新状态</b>：将 wkf_supply_request.status_enum 从 PENDING 更新为 COMPLETED</li>
     * </ol>
     *
     * @param dto 退药确认参数（退药单ID、患者信息、勾选的药品明细ID列表、操作人）
     * @throws IllegalArgumentException 单据不存在、状态不合法、明细不匹配、库存记录缺失或并发冲突时抛出
     */
    @Override
    @Transactional
    public void confirmReturn(ReturnConfirmDTO dto) {
        // ========== 1. 校验退药主单据 ==========
        WkfSupplyRequest supplyRequest = wkfSupplyRequestMapper.selectById(dto.getReturnId());
        if (supplyRequest == null) {
            throw new IllegalArgumentException("退药单据不存在");
        }
        if (!"RETURN_OUTPATIENT".equals(supplyRequest.getTypeEnum())) {
            throw new IllegalArgumentException("该单据不是门诊退药单据");
        }
        if (!"PENDING".equals(supplyRequest.getStatusEnum())) {
            throw new IllegalArgumentException("该退药单据已处理，不允许重复确认");
        }

        // ========== 2. 校验退药明细 ==========
        if (dto.getItemIds() == null || dto.getItemIds().isEmpty()) {
            throw new IllegalArgumentException("请至少勾选一条退药明细");
        }
        List<WkfSupplyDelivery> deliveries = wkfSupplyDeliveryMapper.selectBatchIds(dto.getItemIds());
        if (deliveries.isEmpty()) {
            throw new IllegalArgumentException("退药明细不存在");
        }
        for (WkfSupplyDelivery delivery : deliveries) {
            if (!dto.getReturnId().equals(delivery.getRequestId())) {
                throw new IllegalArgumentException("退药明细 " + delivery.getId() + " 不属于该退药单");
            }
        }

        // ========== 3. 遍历明细，回补库存 ==========
        // 以退药主单据的 purpose_location_id 作为库存库房定位依据
        String locationId = supplyRequest.getPurposeLocationId();
        for (WkfSupplyDelivery delivery : deliveries) {
            // 3a. 按药品ID + 批号 + 库房定位库存记录
            WkfInventoryItem inventoryItem = wkfInventoryItemMapper.findByItemLotAndLocation(
                    delivery.getItemId(),
                    delivery.getLotNumber(),
                    locationId);
            if (inventoryItem == null) {
                throw new IllegalArgumentException(
                        "未找到药品库存记录：药品ID=" + delivery.getItemId()
                                + "，批号=" + delivery.getLotNumber()
                                + "，库房=" + locationId);
            }
            // 3b. 乐观锁更新库存数量（delta 为正数 = 回补）
            int inventoryUpdated = wkfInventoryItemMapper.updateQuantity(
                    inventoryItem.getId(),
                    delivery.getQuantity(),      // 退药数量，正数表示库存增加
                    inventoryItem.getVersion(),  // 乐观锁版本号
                    dto.getOperator());
            if (inventoryUpdated == 0) {
                throw new IllegalArgumentException(
                        "库存更新失败（并发冲突），请重试：药品ID=" + delivery.getItemId()
                                + "，批号=" + delivery.getLotNumber());
            }
        }

        // ========== 4. 更新退药主单据状态为"已完成" ==========
        int requestUpdated = wkfSupplyRequestMapper.update(null,
                new LambdaUpdateWrapper<WkfSupplyRequest>()
                        .eq(WkfSupplyRequest::getId, dto.getReturnId())
                        .eq(WkfSupplyRequest::getTypeEnum, "RETURN_OUTPATIENT")
                        .eq(WkfSupplyRequest::getStatusEnum, "PENDING")
                        .set(WkfSupplyRequest::getStatusEnum, "COMPLETED")
                        .set(WkfSupplyRequest::getUpdatedBy, dto.getOperator()));
        if (requestUpdated == 0) {
            throw new IllegalArgumentException("退药单据状态更新失败，可能已被其他操作修改");
        }
    }
}
