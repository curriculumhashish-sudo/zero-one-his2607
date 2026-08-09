#pragma once
#ifndef __CHECK_INPATIENT_ADMISSION_DTO_H_
#define __CHECK_INPATIENT_ADMISSION_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CheckInpatientAdmissionDTO : public oatpp::DTO
{
    DTO_INIT(CheckInpatientAdmissionDTO, DTO);

    API_DTO_FIELD_DEFAULT(String, encounterId, ZH_WORDS_GETTER("doctor-station.inpatient.check.encounterId"));
};

#include OATPP_CODEGEN_END(DTO)

#endif
