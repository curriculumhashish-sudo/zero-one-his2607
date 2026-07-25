package com.zeroone.star.dispense.service.impl;

import cn.hutool.core.util.StrUtil;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.InventoryItem;
import com.zeroone.star.dispense.entity.MedicationDispense;
import com.zeroone.star.dispense.mapper.InventoryItemMapper;
import com.zeroone.star.dispense.mapper.MedicationDispenseMapper;
import com.zeroone.star.dispense.service.IDispensingService;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.query.j7.dispense.DispensingSummaryQuery;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Service
public class DispensingServiceImpl implements IDispensingService {
    @Resource
    private MedicationDispenseMapper medicationDispenseMapper;
    @Resource
    private InventoryItemMapper inventoryItemMapper;
    @Override
    public ExtendPageDTO<DispensingSummaryVO> querySummaryList(DispensingSummaryQuery query) {
        Page<DispensingSummaryVO> page = new Page<>(query.getPageIndex(), query.getPageSize());
        IPage<DispensingSummaryVO> result = medicationDispenseMapper.selectSummaryPage(page,
                query.getSummaryNo(), query.getWardId(), query.getWardName(),
                query.getStatus(), query.getCreateTimeStart(), query.getCreateTimeEnd());
        return new ExtendPageDTO<>(result.getRecords(), query.getPageIndex(), query.getPageSize(), result.getTotal());
    }

    @Override
    public List<PatientMedicationVO> queryPatientMedications(String summaryNo) {
        return medicationDispenseMapper.selectPatientMedications(summaryNo);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public String verifyDispense(DispenseVerifyDTO verifyDTO) {
        if (StrUtil.isBlank(verifyDTO.getSummaryNo())) {
            throw new IllegalArgumentException("汇总单号不能为空");
        }
        if (verifyDTO.getItems() == null || verifyDTO.getItems().isEmpty()) {
            throw new IllegalArgumentException("发药明细不能为空");
        }

        List<String> failedIds = new ArrayList<>();

        for (DispenseVerifyDTO.VerifyItem item : verifyDTO.getItems()) {
            if (item.getDispenseQuantity() == null
                    || item.getDispenseQuantity().compareTo(BigDecimal.ZERO) <= 0) {
                throw new IllegalArgumentException("实发数量必须大于0，记录ID：" + item.getDispenseId());
            }

            MedicationDispense dispense = medicationDispenseMapper.selectById(item.getDispenseId());
            if (dispense == null || !"PENDING".equals(dispense.getStatusEnum())) {
                failedIds.add(item.getDispenseId());
                continue;
            }
            if (dispense.getQuantity().compareTo(item.getDispenseQuantity()) < 0) {
                failedIds.add(item.getDispenseId());
                continue;
            }

            int rows = medicationDispenseMapper.update(null,
                    new LambdaUpdateWrapper<MedicationDispense>()
                            .eq(MedicationDispense::getId, item.getDispenseId())
                            .eq(MedicationDispense::getStatusEnum, "PENDING")
                            .ge(MedicationDispense::getQuantity, item.getDispenseQuantity())
                            .set(MedicationDispense::getDispenseQuantity, item.getDispenseQuantity())
                            .set(MedicationDispense::getLotNumber, item.getLotNumber())
                            .set(MedicationDispense::getStatusEnum, "COMPLETED")
                            .set(MedicationDispense::getDispenseTime, LocalDateTime.now()));
            if (rows == 0) {
                failedIds.add(item.getDispenseId());
                continue;
            }

            int invRows = inventoryItemMapper.update(null,
                    new LambdaUpdateWrapper<InventoryItem>()
                            .eq(InventoryItem::getItemTable, "med_medication_definition")
                            .eq(InventoryItem::getItemId, dispense.getMedicationId())
                            .eq(InventoryItem::getLotNumber, item.getLotNumber())
                            .ge(InventoryItem::getQuantity, item.getDispenseQuantity())
                            .setSql("quantity = quantity - " + item.getDispenseQuantity()));
            if (invRows == 0) {
                throw new RuntimeException("库存不足，药品ID：" + dispense.getMedicationId()
                        + "，批号：" + item.getLotNumber());
            }
        }

        if (!failedIds.isEmpty()) {
            throw new RuntimeException("以下记录发药失败（已发药或超卖）：" + String.join(",", failedIds));
        }
        return "发药成功";
    }
}
