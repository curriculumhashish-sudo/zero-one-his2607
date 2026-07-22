package com.zero.star.dispense.service;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;

public interface PatientService {
    public IPage<PatientQueueVO> pagePatientQueue(PatientQueueQuery query) ;
}
