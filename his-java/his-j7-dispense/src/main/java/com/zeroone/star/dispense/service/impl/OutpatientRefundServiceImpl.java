package com.zeroone.star.dispense.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.InventoryItem;
import com.zeroone.star.dispense.entity.MedMedicationDispense;
import com.zeroone.star.dispense.mapper.InventoryItemMapper;
import com.zeroone.star.dispense.mapper.MedMedicationDispenseMapper;
import com.zeroone.star.dispense.service.IOutpatientRefundService;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.dto.j7.pharmacy.ReturnRejectDTO;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：门诊退药服务实现类
 * </p>
 * <p>
 * 功能说明：实现门诊退药业务功能，包括：
 * 1. 分页查询门诊退药患者队列（待退药/已退药）
 * 2. 获取患者退药明细信息
 * 3. 确认退药（回补库存）
 * 涉及的表包括：
 * - med_medication_dispense（药品发放记录表）：退药明细
 * - wkf_inventory_item（库存项表）：库存回补
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Service
public class OutpatientRefundServiceImpl implements IOutpatientRefundService {

    @Resource
    private MedMedicationDispenseMapper medicationDispenseMapper;

    @Resource
    private InventoryItemMapper inventoryItemMapper;

    /**
     * 分页查询门诊退药患者队列
     * <p>
     * 查询 med_medication_dispense 表中发放类型为"退药"(REFUND)的记录，
     * 按状态和创建时间条件过滤，分页返回。
     * 状态为"待处理"(PENDING)表示待退药，状态为"已完成"(REFUNDED)表示已退药。
     * </p>
     *
     * @param query 分页查询参数
     * @return 患者队列分页数据
     */
    @Override
    public IPage<PatientQueueVO> queryRefundPatientQueue(PatientQueueQuery query) {
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        long pageSize = query.getPageSize() > 0 ? query.getPageSize() : 10;

        Page<PatientQueueVO> page = new Page<>(pageIndex, pageSize);

        // 设置退药类型
        query.setDispenseEnum("REFUND");

        return medicationDispenseMapper.selectRefundPatientPage(page, query);
    }

    /**
     * 分页获取患者退药明细信息
     * <p>
     * 分页查询 med_medication_dispense 表中指定患者的退药明细记录。
     * </p>
     *
     * @param query 查询参数（包含患者ID、就诊ID、分页参数等）
     * @return 药品明细分页数据
     */
    @Override
    public IPage<PatientDrugItemVO> queryRefundDrugItems(PatientDrugItemQuery query) {
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        long pageSize = query.getPageSize() > 0 ? query.getPageSize() : 10;

        Page<PatientDrugItemVO> page = new Page<>(pageIndex, pageSize);

        // 设置退药类型
        query.setDispenseEnum("REFUND");

        return medicationDispenseMapper.selectRefundDrugItems(page, query);
    }

    /**
     * 执行确认退药（回补库存）
     * <p>
     * 根据勾选的药品明细ID列表，回补 wkf_inventory_item 表库存数量，
     * 并更新 med_medication_dispense 表状态为"已退药"(REFUNDED)。
     * 业务流程：
     * 1. 校验退药明细存在且状态为"待处理"(PENDING)
     * 2. 根据明细ID查询 med_medication_dispense 获取药品信息
     * 3. 根据药品ID、批号查询 wkf_inventory_item 库存项
     * 4. 回补库存数量（增加 quantity）
     * 5. 更新 med_medication_dispense 状态为"已退药"(REFUNDED)
     * </p>
     *
     * @param dto 退药确认参数
     * @return 操作结果
     */
    @Override
    @Transactional(rollbackFor = Exception.class)
    public String confirmRefund(DispenseVerifyDTO dto) {
        if (StrUtil.isBlank(dto.getSummaryNo())) {
            throw new IllegalArgumentException("退药单号不能为空");
        }
        if (dto.getItems() == null || dto.getItems().isEmpty()) {
            throw new IllegalArgumentException("退药明细不能为空");
        }

        List<String> failedIds = new ArrayList<>();

        for (DispenseVerifyDTO.VerifyItem item : dto.getItems()) {
            if (item.getDispenseQuantity() == null
                    || item.getDispenseQuantity().compareTo(BigDecimal.ZERO) <= 0) {
                throw new IllegalArgumentException("退药数量必须大于0，记录ID：" + item.getDispenseId());
            }

            // 1. 查询退药明细
            MedMedicationDispense dispense = medicationDispenseMapper.selectById(item.getDispenseId());
            if (dispense == null || !"PENDING".equals(dispense.getStatusEnum())) {
                failedIds.add(item.getDispenseId());
                continue;
            }

            // 校验发放类型为退药
            if (!"REFUND".equals(dispense.getDispenseEnum())) {
                throw new IllegalArgumentException("记录不是退药类型，ID：" + item.getDispenseId());
            }

            if (dispense.getQuantity().compareTo(item.getDispenseQuantity()) < 0) {
                failedIds.add(item.getDispenseId());
                continue;
            }

            // 2. 回补库存数量
            int invRows = inventoryItemMapper.update(null,
                    new LambdaUpdateWrapper<InventoryItem>()
                            .eq(InventoryItem::getItemTable, "med_medication_definition")
                            .eq(InventoryItem::getItemId, dispense.getMedicationId())
                            .eq(InventoryItem::getLotNumber, item.getLotNumber())
                            .setSql("quantity = quantity + " + item.getDispenseQuantity()));
            if (invRows == 0) {
                throw new IllegalArgumentException("库存回补失败，药品ID：" + dispense.getMedicationId()
                        + "，批号：" + item.getLotNumber());
            }

            // 3. 更新退药明细状态为"已退药"
            int rows = medicationDispenseMapper.update(null,
                    new LambdaUpdateWrapper<MedMedicationDispense>()
                            .eq(MedMedicationDispense::getId, item.getDispenseId())
                            .eq(MedMedicationDispense::getStatusEnum, "PENDING")
                            .set(MedMedicationDispense::getDispenseQuantity, item.getDispenseQuantity())
                            .set(MedMedicationDispense::getLotNumber, item.getLotNumber())
                            .set(MedMedicationDispense::getStatusEnum, "REFUNDED")
                            .set(MedMedicationDispense::getDispenseTime, LocalDateTime.now()));
            if (rows == 0) {
                failedIds.add(item.getDispenseId());
                continue;
            }
        }

        if (!failedIds.isEmpty()) {
            throw new IllegalArgumentException("以下记录退药失败（已退药或数量异常）：" + String.join(",", failedIds));
        }
        return "退药成功";
    }

    /**
     * 执行拒绝退药
     * <p>
     * 更新 med_medication_dispense 表：设置状态为"已拒绝"(DECLINED)，记录拒绝原因。
     * 同时校验退药类型为"退药"(REFUND)，确保不会误操作发药记录。
     * 优先使用 itemIds 批量拒绝，若为空则使用 returnId 单条拒绝。
     * </p>
     *
     * @param dto 拒绝退药参数
     */
    @Override
    @Transactional
    public void rejectReturn(ReturnRejectDTO dto) {
        LambdaUpdateWrapper<MedMedicationDispense> wrapper = new LambdaUpdateWrapper<MedMedicationDispense>()
                .eq(MedMedicationDispense::getDispenseEnum, "REFUND")
                .eq(MedMedicationDispense::getStatusEnum, "PENDING")
                .set(MedMedicationDispense::getStatusEnum, "DECLINED")
                .set(MedMedicationDispense::getNotPerformedReasonEnum, dto.getRejectReason());
        if (dto.getItemIds() != null && !dto.getItemIds().isEmpty()) {
            wrapper.in(MedMedicationDispense::getId, dto.getItemIds());
        } else {
            wrapper.eq(MedMedicationDispense::getId, dto.getReturnId());
        }
        medicationDispenseMapper.update(null, wrapper);
    }
}
