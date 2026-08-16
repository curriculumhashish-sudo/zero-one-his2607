#pragma once

#ifndef _CHECK_INPATIENT_ADMISSION_SERVICE_H_
#define _CHECK_INPATIENT_ADMISSION_SERVICE_H_

#include "domain/dto/doctor/CheckInpatientAdmissionDTO.h"
#include "domain/vo/doctor/CheckInpatientAdmissionVO.h"

class CheckInpatientAdmissionService
{
public:
    oatpp::Object<CheckInpatientAdmissionVO> check(const CheckInpatientAdmissionDTO::Wrapper& dto);
};

#endif
