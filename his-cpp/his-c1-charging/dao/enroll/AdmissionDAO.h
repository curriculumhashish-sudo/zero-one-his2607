#pragma once

#ifndef _ADMISSION_DAO_
#define _ADMISSION_DAO_

#include "BaseDAO.h"

// Admission data access object.
// Handles multi-table insert/delete operations for inpatient admission registration.
class AdmissionDAO : public BaseDAO
{
public:
	// Insert a new inpatient admission record (multi-table transaction).
	// Returns true on success; outPatientId/outEncounterId/outBusNo receive the generated IDs.
	bool insertAdmission(
		const std::string& name,
		const std::string& idCard,
		const std::string& birthTime,
		int age,
		const std::string& sex,
		const std::string& phone,
		const std::string& admissionDept,
		const std::string& admissionWard,
		const std::string& attendingDoctor,
		const std::string& admissionDiagnosis,
		const std::string& diagnosisDesc,
		const std::string& patientCondition,
		const std::string& admissionType,
		const std::string& admissionMode,
		const std::string& feeNature,
		const std::string& admissionDate,
		const std::string& prepayAmount,
		const std::string& payWay,
		const std::string& tenantId,
		const std::string& orgId,
		const std::string& operUserId,
		std::string& outPatientId,
		std::string& outEncounterId,
		std::string& outBusNo
	);

	// Cancel an inpatient admission (soft delete: set is_deleted = 1).
	// Returns true on success.
	bool deleteAdmission(
		const std::string& encounterId,
		const std::string& cancelReason,
		const std::string& operUserId,
		const std::string& tenantId
	);

private:
	// Generate a UUID string without separators
	std::string generateUuid();

	// Generate a business number, e.g. "ZY202607280001"
	std::string generateBusNo(const std::string& prefix);

	// Append a string-type SqlParam.
	static void sp(SqlParams& p, const std::string& v);
};

#endif // !_ADMISSION_DAO_
