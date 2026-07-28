package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.MedMedicationDefinitionMapper;
import com.zeroone.star.dispense.service.DrugService;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

@Service
public class DrugServiceImpl implements DrugService {

    @Resource
    private MedMedicationDefinitionMapper drugMapper;

    @Override
    public IPage<PatientDrugItemVO> pagePatientDrugItems(PatientDrugItemQuery query) {
        Page<?> page = new Page<>(query.getPageIndex(), query.getPageSize());
        return drugMapper.pagePatientDrugItems(page, query);
    }
}