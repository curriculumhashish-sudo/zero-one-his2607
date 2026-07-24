package com.zeroone.star.j3.patient.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j3.patient.PatientCreateDTO;
import com.zeroone.star.project.dto.j3.patient.PatientUpdateDTO;
import com.zeroone.star.project.query.j3.patient.PatientQuery;
import com.zeroone.star.project.vo.j3.patient.PatientListVO;

/**
 * 患者管理业务服务。
 *
 * @author 照夜清
 */
public interface PatientService {

    PageDTO<PatientListVO> queryPatientPage(PatientQuery query);

    String createPatient(PatientCreateDTO dto);

    void updatePatient(String id, PatientUpdateDTO dto);
}
