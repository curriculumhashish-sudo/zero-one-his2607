package com.zero.star.outpharm.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zero.star.outpharm.service.PatientService;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;

public class PatientServiceImpl implements PatientService {
    @Override
    public IPage<PatientQueueVO> pagePatientQueue(PatientQueueQuery query) {
        return null;
    }
}
