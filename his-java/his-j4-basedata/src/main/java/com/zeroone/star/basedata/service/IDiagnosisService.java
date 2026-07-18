package com.zeroone.star.basedata.service;

import com.baomidou.mybatisplus.extension.service.IService;
import com.zeroone.star.basedata.entity.Diagnosis;
import com.zeroone.star.project.dto.j4.diagnosis.CreateDiagnosisDTO;
import com.zeroone.star.project.dto.j4.diagnosis.UpdateDiagnosisDTO;
import com.zeroone.star.project.vo.j4.diagnosis.DiagnosisVO;

public interface IDiagnosisService extends IService<Diagnosis> {

    DiagnosisVO createDiagnosis(CreateDiagnosisDTO request);

    DiagnosisVO updateDiagnosis(String id, UpdateDiagnosisDTO request);

    void deleteDiagnosis(String id);

    DiagnosisVO toggleDiagnosisStatus(String id, String valiFlag);
}