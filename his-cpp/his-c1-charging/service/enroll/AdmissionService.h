#pragma once
#ifndef _ADMISSION_SERVICE_
#define _ADMISSION_SERVICE_

#include "domain/dto/enroll/admissionDTO.h"
#include "domain/dto/PayloadDTO.h"
#include <string>

// Result struct returned by AdmissionService::insertAdmission.
// code is empty on success, or a failure code ("DATE_INVALID", "INSERT_FAILED").
struct AdmissionInsertResult
{
	std::string code;
	std::string patientId;
	std::string encounterId;
	std::string encounterBusNo;
};

class AdmissionService
{
public:
	// Inpatient admission registration.
	AdmissionInsertResult insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload);

	// Cancel admission registration.
	// Returns an empty string on success, or a failure code
	// ("ENCOUNTER_NOT_FOUND", "STATUS_NOT_CANCELABLE", "DELETE_FAILED").
	std::string deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload);
};

#endif // !_ADMISSION_SERVICE_
