#pragma once
#ifndef _ADMISSION_SERVICE_
#define _ADMISSION_SERVICE_

#include "domain/dto/enroll/admissionDTO.h"
#include "domain/dto/PayloadDTO.h"
#include <string>

class AdmissionService
{
public:
	// Inpatient admission registration.
	// Returns an empty string on success, or a failure code
	// ("DATE_INVALID", "INSERT_FAILED").
	std::string insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload);

	// Cancel admission registration.
	// Returns an empty string on success, or a failure code
	// ("ENCOUNTER_NOT_FOUND", "STATUS_NOT_CANCELABLE", "DELETE_FAILED").
	std::string deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_ADMISSION_SERVICE_
