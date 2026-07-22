package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.DrugService;
import com.zeroone.star.project.query.j7.outpharm.PatientDrugItemQuery;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;
import org.springframework.stereotype.Service;

@Service
public class DrugServiceImpl implements DrugService {
    @Override
    public IPage<PatientDrugItemVO> pagePatientDrugItems(PatientDrugItemQuery query) {
        return null;
    }
}
