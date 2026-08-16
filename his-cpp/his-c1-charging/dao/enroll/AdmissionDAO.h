#pragma once

#ifndef _ADMISSION_DAO_
#define _ADMISSION_DAO_

#include "BaseDAO.h"
#include <string>
#include <vector>

// Prepayment payment detail item to be persisted into fin_payment_rec_detail.
struct AdmissionPaymentDetail
{
	std::string payWay;
	std::string amount;
};

// Admission data access object.
// Handles multi-table insert/delete operations for inpatient admission registration.
class AdmissionDAO : public BaseDAO
{
public:
	// Insert a new inpatient admission record (multi-table transaction).
	// Returns an empty string on success, or a failure code on error
	// ("DATE_INVALID", "INSERT_FAILED").
	// outPatientId/outEncounterId/outBusNo receive the generated IDs.
	std::string insertAdmission(
		const std::string& name,
		const std::string& idCard,
		const std::string& birthTime,
		int age,
		const std::string& sex,
		const std::string& phone,
		const std::string& organizationId,
		const std::string& wardId,
		const std::string& attendingDoctorId,
		const std::string& contractId,
		const std::string& admissionDiagnosis,
		const std::string& diagnosisDesc,
		const std::string& patientCondition,
		const std::string& admissionType,
		const std::string& admissionMode,
		const std::string& feeNature,
		const std::string& admissionDate,
		const std::string& prepayAmount,
		const std::vector<AdmissionPaymentDetail>& paymentDetails,
		const std::string& tenantId,
		const std::string& operUserId,
		std::string& outPatientId,
		std::string& outEncounterId,
		std::string& outBusNo
	);

	// Cancel an inpatient admission (soft delete: set is_deleted = 1,
	// persist cancel reason into adm_encounter_reason).
	// Returns an empty string on success, or a failure code
	// ("ENCOUNTER_NOT_FOUND", "STATUS_NOT_CANCELABLE", "DELETE_FAILED").
	std::string deleteAdmission(
		const std::string& encounterId,
		const std::string& cancelReason,
		bool insuranceRevoked,
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
