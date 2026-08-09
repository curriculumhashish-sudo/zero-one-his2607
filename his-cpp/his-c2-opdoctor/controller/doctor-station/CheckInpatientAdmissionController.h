#pragma once

#ifndef _CHECK_INPATIENT_ADMISSION_CONTROLLER_H_
#define _CHECK_INPATIENT_ADMISSION_CONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/doctor/CheckInpatientAdmissionVO.h"
#include "domain/dto/doctor/CheckInpatientAdmissionDTO.h"

#include OATPP_CODEGEN_BEGIN(ApiController)
#define API_TAG_CHK ZH_WORDS_GETTER("doctor-station.inpatient.check.tag")

class CheckInpatientAdmissionController : public oatpp::web::server::api::ApiController
{
    API_ACCESS_DECLARE(CheckInpatientAdmissionController);
public:
    API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("doctor-station.inpatient.check.summary"),
        checkInpatientAdmission, CheckInpatientAdmissionJsonVO::Wrapper, API_TAG_CHK);
    API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/doctor-station/inpatient-admission/check",
        checkInpatientAdmission,
        BODY_DTO(CheckInpatientAdmissionDTO::Wrapper, dto),
        execCheck(dto));
private:
    CheckInpatientAdmissionJsonVO::Wrapper execCheck(const CheckInpatientAdmissionDTO::Wrapper& dto);
};
#undef API_TAG_CHK

#include OATPP_CODEGEN_END(ApiController)
#endif
