#pragma once
#ifndef _ADMISSION_SERVICE_
#define _ADMISSION_SERVICE_

#include "domain/dto/enroll/admissionDTO.h"
#include "domain/dto/PayloadDTO.h"

class AdmissionService
{
public:
	// Inpatient admission registration
	bool insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload);

	// Cancel admission registration
	bool deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_ADMISSION_SERVICE_
