#include "stdafx.h"
#include "AdmissionService.h"
#include "../../dao/enroll/AdmissionDAO.h"
#include "id/UuidFacade.h"

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

// Inpatient admission registration
bool AdmissionService::insertAdmission(const AdmissionAddDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate required fields
	std::string errMsg = dto->validate();
	if (!errMsg.empty()) return false;

	// Extract fields from DTO and convert to std::string
	std::string name = toStr(dto->name);
	std::string idCard = toStr(dto->idCard);
	std::string birthTime = toStr(dto->birthtime);
	int age = dto->age ? dto->age.getValue(0) : 0;
	std::string sex = toStr(dto->sex);
	std::string phone = toStr(dto->phone);
	std::string admissionDept = toStr(dto->admissionDept);
	std::string admissionWard = toStr(dto->admission_ward);
	std::string attendingDoctor = toStr(dto->attending_doctor);
	std::string admissionDiagnosis = toStr(dto->admissionDiagnosis);
	std::string diagnosisDesc = toStr(dto->diagnosisDesc);
	std::string patientCondition = toStr(dto->patientCondition);
	std::string admissionType = toStr(dto->admissionType);
	std::string admissionMode = toStr(dto->admissionMode);
	std::string feeNature = toStr(dto->feeNature);
	std::string admissionDate = toStr(dto->admissionDate);
	std::string prepayAmount = toStr(dto->prepayAmount, "0");
	std::string payWay = toStr(dto->payWay, "CASH");

	// Extract auth info from payload
	// TODO: tenantId should be extracted from JWT custom claims, not from payload.getId()
	std::string tenantId = payload.getId();
	std::string orgId = "1";       // TODO: extract from JWT organization claim
	std::string operUserId = payload.getId();

	// Output parameters
	std::string outPatientId;
	std::string outEncounterId;
	std::string outBusNo;

	// Call DAO to insert admission record
	AdmissionDAO dao;
	return dao.insertAdmission(
		name, idCard, birthTime, age, sex, phone,
		admissionDept, admissionWard, attendingDoctor,
		admissionDiagnosis, diagnosisDesc, patientCondition,
		admissionType, admissionMode, feeNature, admissionDate,
		prepayAmount, payWay, tenantId, orgId, operUserId,
		outPatientId, outEncounterId, outBusNo
	);
}

// Cancel admission registration
bool AdmissionService::deleteAdmission(const AdmissionDeleteDTO::Wrapper& dto, const PayloadDTO& payload)
{
	// Validate required fields
	std::string errMsg = dto->validate();
	if (!errMsg.empty()) return false;

	// Extract encounter ID from delete DTO
	std::string encounterId = toStr(dto->encounterId);
	std::string cancelReason = "Manual cancellation";

	// Extract auth info from payload
	std::string tenantId = payload.getId();
	std::string operUserId = payload.getId();

	// Call DAO to delete (cancel) admission record
	AdmissionDAO dao;
	return dao.deleteAdmission(encounterId, cancelReason, operUserId, tenantId);
}
