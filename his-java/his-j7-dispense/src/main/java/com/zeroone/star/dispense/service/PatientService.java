package com.zeroone.star.dispense.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;

public interface PatientService {
    public IPage<PatientQueueVO> pagePatientQueue(PatientQueueQuery query) ;
}
