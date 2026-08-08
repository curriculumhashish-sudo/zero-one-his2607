#include "stdafx.h"
#include "AdmissionService.h"
#include "../../dao/enroll/AdmissionDAO.h"
#include "id/UuidFacade.h"

#include <sstream>
#include <iomanip>

// Nacos client, included to prevent symbol conflicts
#include "NacosClient.h"

// Convert oatpp::String field to std::string, with fallback for null
static std::string toStr(const oatpp::String& field, const std::string& fallback = "")
{
	return field ? field.getValue("") : fallback;
}

// Convert oatpp numeric field to string, with fallback for null
static std::string toNumericStr(const oatpp::Int64& field, const std::string& fallback = "")
{
	return field ? std::to_string(field.getValue(0)) : fallback;
}

// Inpatient admission registration.
// Returns an empty string on success, or a failure code on error.
std::string AdmissionService::insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate required fields
	std::string errMsg = dto->validate();
	if (!errMsg.empty()) return "PARAMS_INVALID";

	// Extract fields from DTO and convert to std::string
	std::string name = toStr(dto->name);
	std::string idCard = toStr(dto->idCard);
	std::string birthTime = toStr(dto->birthtime);
	int age = dto->age ? dto->age.getValue(0) : 0;
	std::string sex = toStr(dto->sex);
	std::string phone = toStr(dto->phone);

	// New ID fields take precedence; legacy fields fall back when new ones are empty
	std::string organizationId = toStr(dto->organizationId, toStr(dto->admissionDept, "1"));
	std::string wardId = toStr(dto->wardId, toStr(dto->admission_ward));
	std::string attendingDoctorId = toStr(dto->attendingDoctorId, toStr(dto->attending_doctor));
	std::string contractId = toStr(dto->contractId);

	std::string admissionDiagnosis = toStr(dto->admissionDiagnosis);
	std::string diagnosisDesc = toStr(dto->diagnosisDesc);
	std::string patientCondition = toStr(dto->patientCondition);
	std::string admissionType = toStr(dto->admissionType);
	std::string admissionMode = toStr(dto->admissionMode);
	std::string feeNature = toStr(dto->feeNature);
	std::string admissionDate = toStr(dto->admissionDate);
	std::string prepayAmount = toStr(dto->prepayAmount);

	// Build payment detail list; when details exist, total amount falls back to prepayAmount
	std::vector<AdmissionPaymentDetail> paymentDetails;

	if (dto->paymentDetails && !dto->paymentDetails->empty())
	{
		double totalAmount = 0.0;

		for (const auto& detail : *dto->paymentDetails.get())
		{
			if (!detail) continue;

			AdmissionPaymentDetail item;
			item.payWay = toStr(detail->payWay, "CASH");
			item.amount = toStr(detail->amount, "0");
			paymentDetails.push_back(item);

			try
			{
				totalAmount += std::stod(item.amount);
			}
			catch (const std::exception&)
			{
				// ignore invalid amount
			}
		}

		if (prepayAmount.empty() && !paymentDetails.empty())
		{
			std::ostringstream amountStream;
			amountStream << std::setprecision(6) << totalAmount;
			prepayAmount = amountStream.str();
		}
	}

	// Extract auth info from payload
	// TODO: tenantId should be extracted from JWT custom claims, not from payload.getId()
	std::string tenantId = payload.getId();
	std::string operUserId = payload.getId();

	// Output parameters
	std::string outPatientId;
	std::string outEncounterId;
	std::string outBusNo;

	// Call DAO to insert admission record
	AdmissionDAO dao;
	return dao.insertAdmission(
		name, idCard, birthTime, age, sex, phone,
		organizationId, wardId, attendingDoctorId, contractId,
		admissionDiagnosis, diagnosisDesc, patientCondition,
		admissionType, admissionMode, feeNature, admissionDate,
		prepayAmount, paymentDetails, tenantId, operUserId,
		outPatientId, outEncounterId, outBusNo
	);
}

// Cancel admission registration.
// Returns an empty string on success, or a failure code on error.
std::string AdmissionService::deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate required fields
	std::string errMsg = dto->validate();
	if (!errMsg.empty()) return "PARAMS_INVALID";

	// Extract encounter ID and cancel reason from delete DTO
	std::string encounterId = toStr(dto->encounterId);
	std::string cancelReason = toStr(dto->cancelReason, "Manual cancellation");
	bool insuranceRevoked = dto->insuranceRevoked ? dto->insuranceRevoked.getValue(false) : false;

	// Extract auth info from payload
	std::string tenantId = payload.getId();
	std::string operUserId = payload.getId();

	// Call DAO to delete (cancel) admission record
	AdmissionDAO dao;
	return dao.deleteAdmission(encounterId, cancelReason, insuranceRevoked, operUserId, tenantId);
}
