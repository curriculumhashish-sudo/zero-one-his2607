package com.zeroone.star.dispense.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.query.j7.dispense.PatientDrugItemQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientDrugItemVO;

public interface DrugService {
    public IPage<PatientDrugItemVO> pagePatientDrugItems(PatientDrugItemQuery query) ;
}
