package com.zero.star.outpharm.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.query.j7.outpharm.PatientDrugItemQuery;
import com.zeroone.star.project.vo.j7.outpharm.PatientDrugItemVO;

public interface DrugService {
    public IPage<PatientDrugItemVO> pagePatientDrugItems(PatientDrugItemQuery query) ;
}
