package com.zeroone.star.ons.service;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.ExecutionRecordVO;
import com.zeroone.star.project.vo.j5.ons.PatientQueueVO;

import java.util.List;

public interface IOnsService {

    JsonVO<Boolean> execute(ExecuteRequest request);

    JsonVO<Boolean> cancelExecute(CancelExecuteRequest request);

    JsonVO<String> print(PrintRequest request);

    JsonVO<PageDTO<ExecutionRecordVO>> getServiceRequests(ExecutionRecordQuery query);

    JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query);

    JsonVO<List<ExecutionRecordVO>> getPatientServiceRequests(String patientId);
}