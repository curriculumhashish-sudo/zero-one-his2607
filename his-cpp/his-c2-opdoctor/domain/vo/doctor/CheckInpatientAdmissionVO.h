#pragma once
#ifndef __CHECK_INPATIENT_ADMISSION_VO_H_
#define __CHECK_INPATIENT_ADMISSION_VO_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/dto/doctor/CheckInpatientAdmissionDTO.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CheckInpatientAdmissionVO : public oatpp::DTO
{
    DTO_INIT(CheckInpatientAdmissionVO, DTO);

    API_DTO_FIELD_DEFAULT(Boolean, exists, ZH_WORDS_GETTER("doctor-station.inpatient.check.exists"));
    API_DTO_FIELD_DEFAULT(String, applicationId, ZH_WORDS_GETTER("doctor-station.inpatient.check.applicationId"));
    API_DTO_FIELD_DEFAULT(String, inpatientEncounterId, ZH_WORDS_GETTER("doctor-station.inpatient.check.inpatientEncounterId"));
    API_DTO_FIELD_DEFAULT(String, inpatientEncounterNo, ZH_WORDS_GETTER("doctor-station.inpatient.check.inpatientEncounterNo"));
    API_DTO_FIELD_DEFAULT(String, status, ZH_WORDS_GETTER("doctor-station.inpatient.check.status"));
    API_DTO_FIELD_DEFAULT(String, statusName, ZH_WORDS_GETTER("doctor-station.inpatient.check.statusName"));
};

class CheckInpatientAdmissionJsonVO : public JsonVO<CheckInpatientAdmissionVO::Wrapper>
{
    DTO_INIT(CheckInpatientAdmissionJsonVO, JsonVO<CheckInpatientAdmissionVO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
#endif
