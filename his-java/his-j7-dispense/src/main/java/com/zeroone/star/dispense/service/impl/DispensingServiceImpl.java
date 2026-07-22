package com.zeroone.star.dispense.service.impl;

import com.zeroone.star.dispense.service.IDispensingService;
import com.zeroone.star.project.dto.ExtendPageDTO;
import com.zeroone.star.project.dto.j7.dispense.DispenseVerifyDTO;
import com.zeroone.star.project.query.j7.dispense.DispensingSummaryQuery;
import com.zeroone.star.project.vo.j7.dispense.DispensingSummaryVO;
import com.zeroone.star.project.vo.j7.dispense.PatientMedicationVO;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class DispensingServiceImpl implements IDispensingService {
    @Override
    public ExtendPageDTO<DispensingSummaryVO> querySummaryList(DispensingSummaryQuery query) {
        return null;
    }

    @Override
    public List<PatientMedicationVO> queryPatientMedications(Long summaryId) {
        return List.of();
    }

    @Override
    public String verifyDispense(DispenseVerifyDTO verifyDTO) {
        return "";
    }

    @Override
    public String closeSummary(String summaryNo) {
        return "";
    }
}
