package com.zeroone.star.project.j5.ons;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j5.ons.*;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j5.ons.ExecutionRecordVO;
import com.zeroone.star.project.vo.j5.ons.PatientQueueVO;

import java.util.List;

public interface OnsApis {

    default JsonVO<Boolean> execute(ExecuteRequest request) {
        return null;
    }

    default JsonVO<Boolean> cancelExecute(CancelExecuteRequest request) {
        return null;
    }

    default JsonVO<String> print(PrintRequest request) {
        return null;
    }

    default JsonVO<PageDTO<ExecutionRecordVO>> getServiceRequests(ExecutionRecordQuery query) {
        return null;
    }

    default JsonVO<PageDTO<PatientQueueVO>> getPatientQueue(PatientQueueQuery query) {
        return null;
    }

    default JsonVO<List<ExecutionRecordVO>> getPatientServiceRequests(String patientId) {
        return null;
    }
}