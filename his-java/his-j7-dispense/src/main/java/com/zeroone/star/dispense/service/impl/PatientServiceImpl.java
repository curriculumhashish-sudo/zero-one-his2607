package com.zeroone.star.dispense.service.impl;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.dispense.mapper.AdmPatientMapper;
import com.zeroone.star.dispense.service.PatientService;
import com.zeroone.star.project.query.j7.dispense.PatientQueueQuery;
import com.zeroone.star.project.vo.j7.dispense.PatientQueueVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

/**
 * <p>
 *   获取患者队列
 * </p>
 * @author xingfu
 */
@Service
public class PatientServiceImpl implements PatientService {



    @Resource
    private AdmPatientMapper patientMapper;

    @Override
    public IPage<PatientQueueVO> pagePatientQueue(PatientQueueQuery query) {
        long pageIndex = query.getPageIndex() > 0 ? query.getPageIndex() : 1;
        long pageSize = query.getPageSize() > 0 ? query.getPageSize() : 10;

        Page<?> page = new Page<>(pageIndex, pageSize);
        return patientMapper.pagePatientQueue(page, query);
    }
}