package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.zeroone.star.dispense.service.PatientService;
import com.zeroone.star.project.query.j7.outpharm.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.outpharm.PatientQueueVO;
import org.springframework.stereotype.Service;

@Service
public class PatientServiceImpl implements PatientService {
    @Override
    public IPage<PatientQueueVO> pagePatientQueue(PatientQueueQuery query) {
        return null;
    }
}
