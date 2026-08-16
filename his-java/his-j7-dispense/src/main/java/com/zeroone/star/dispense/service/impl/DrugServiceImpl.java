package com.zeroone.star.dispense.service.impl;

import cn.hutool.core.collection.CollUtil;
import com.baomidou.mybatisplus.core.conditions.update.LambdaUpdateWrapper;
import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.entity.MedMedicationDispense;
import com.zeroone.star.dispense.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.dispense.mapper.MedMedicationDispenseMapper;
import com.zeroone.star.dispense.service.DrugService;
import com.zeroone.star.project.dto.j7.dispense.BatchDispenseCommand;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.vo.j7.dispense.BatchDispenseResultVO;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Service
public class DrugServiceImpl implements DrugService {

    @Resource
    private MedMedicationDefinitionMapper drugMapper;

    @Resource
    private MedMedicationDispenseMapper dispenseMapper;

    @Override
    public IPage<PatientDrugItemVO> pagePatientDrugItems(PatientDrugItemQuery query) {
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        long pageSize = query.getPageSize() > 0 ? query.getPageSize() : 10;
        Page<?> page = new Page<>(pageIndex, pageSize);
        return drugMapper.pagePatientDrugItems(page, query);
    }

    @Override
    @Transactional(rollbackFor = Exception.class)
    public BatchDispenseResultVO batchDispense(BatchDispenseCommand command) {
        List<String> dispenseIds = command.getDispenseIds();
        String practitionerId = command.getPractitionerId();
        LocalDateTime now = LocalDateTime.now();

        BatchDispenseResultVO result = new BatchDispenseResultVO();
        List<String> successIds = new ArrayList<>();
        List<String> failIds = new ArrayList<>();

        List<MedMedicationDispense> records = dispenseMapper.selectBatchIds(dispenseIds);

        if (CollUtil.isEmpty(records)) {
            result.setSuccessCount(0);
            result.setFailCount(dispenseIds.size());
            result.setFailIds(new ArrayList<>(dispenseIds));
            result.setFailMessage("未找到有效的发药记录");
            return result;
        }

        for (MedMedicationDispense record : records) {
            if (record == null || !"PENDING".equals(record.getStatusEnum())) {
                failIds.add(record != null ? record.getId() : "null");
                continue;
            }
            if (!"OUTPATIENT".equals(record.getDispenseEnum())) {
                failIds.add(record.getId());
                continue;
            }

            int rows = dispenseMapper.update(null,
                    new LambdaUpdateWrapper<MedMedicationDispense>()
                            .eq(MedMedicationDispense::getId, record.getId())
                            .eq(MedMedicationDispense::getStatusEnum, "PENDING")
                            .set(MedMedicationDispense::getStatusEnum, "COMPLETED")
                            .set(MedMedicationDispense::getDispenseQuantity, record.getQuantity())
                            .set(MedMedicationDispense::getPractitionerId, practitionerId)
                            .set(MedMedicationDispense::getDispenseTime, now)
                            .set(MedMedicationDispense::getStatusChangedTime, now)
                            .set(MedMedicationDispense::getUpdatedAt, now));

            if (rows > 0) {
                successIds.add(record.getId());
            } else {
                failIds.add(record.getId());
            }
        }

        for (String id : dispenseIds) {
            if (!successIds.contains(id) && !failIds.contains(id)) {
                failIds.add(id);
            }
        }

        result.setSuccessCount(successIds.size());
        result.setFailCount(failIds.size());
        result.setSuccessIds(successIds);
        result.setFailIds(failIds);
        result.setFailMessage("发药完成：成功 " + successIds.size() + " 条"
                + (failIds.isEmpty() ? "" : "，失败 " + failIds.size() + " 条"));

        return result;
    }
}