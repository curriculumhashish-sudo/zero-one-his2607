#pragma once
#ifndef __REQUEST_INPATIENT_ADMISSION_DTO_H_
#define __REQUEST_INPATIENT_ADMISSION_DTO_H_

#include "AdmissionDiagnosisDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RequestInpatientAdmissionDTO : public oatpp::DTO
{
    DTO_INIT(RequestInpatientAdmissionDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("adm-req.encounterId"));
    API_DTO_FIELD_DEFAULT(String, patientId, ZH_WORDS_GETTER("adm-req.patientId"));
    API_DTO_FIELD_DEFAULT(String, targetDepartmentId, ZH_WORDS_GETTER("adm-req.targetDepartmentId"));
    API_DTO_FIELD_DEFAULT(String, wardId, ZH_WORDS_GETTER("adm-req.wardId"));
    API_DTO_FIELD_DEFAULT(String, admissionTypeCode, ZH_WORDS_GETTER("adm-req.admissionTypeCode"));
    API_DTO_FIELD_DEFAULT(String, admissionWayCode, ZH_WORDS_GETTER("adm-req.admissionWayCode"));
    API_DTO_FIELD_DEFAULT(String, priorityCode, ZH_WORDS_GETTER("adm-req.priorityCode"));
    API_DTO_FIELD_DEFAULT(List<Object<AdmissionDiagnosisDTO>>, admissionDiagnoses, ZH_WORDS_GETTER("adm-req.admissionDiagnoses"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
