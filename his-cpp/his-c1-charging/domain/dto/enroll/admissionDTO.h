#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 10:59:38

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _ADMISSION_DTO_
#define _ADMISSION_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Admission prepayment payment detail item.
 * Submitted inside AdmissionAddDTO.paymentDetails.
 */
class AdmissionPaymentDetailDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionPaymentDetailDTO, DTO);

	// Payment channel code (CASH / WECHAT / ALIPAY / BANK_CARD / ACCOUNT / INSURANCE)
	DTO_FIELD(String, payWay);
	DTO_FIELD_INFO(payWay) {
		info->description = ZH_WORDS_GETTER("charging.admission.payWay");
	}

	// Payment amount
	DTO_FIELD(String, amount);
	DTO_FIELD_INFO(amount) {
		info->description = ZH_WORDS_GETTER("charging.admission.paymentAmount");
	}
};

/**
 * Admission registration data transfer object.
 */
class AdmissionAddDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionAddDTO, DTO);
	// Patient name
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("charging.admission.name");
	}

	// ID card number (18-digit, stored as string)
	DTO_FIELD(String, idCard);
	DTO_FIELD_INFO(idCard) {
		info->description = ZH_WORDS_GETTER("charging.admission.idCard");
	}

	// Date of birth
	DTO_FIELD(String, birthtime);
	DTO_FIELD_INFO(birthtime) {
		info->description = ZH_WORDS_GETTER("charging.admission.birthtime");
	}

	// Age
	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("charging.admission.age");
	}

	// Gender
	DTO_FIELD(String, sex);
	DTO_FIELD_INFO(sex) {
		info->description = ZH_WORDS_GETTER("charging.admission.sex");
	}

	// Phone number (stored as string to preserve leading zeros)
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("charging.admission.phone");
	}

	// Admission department
	DTO_FIELD(String, admissionDept);
	DTO_FIELD_INFO(admissionDept) {
		info->description = ZH_WORDS_GETTER("charging.admission.admissionDept");
	}

	// Admission ward
	DTO_FIELD(String, admission_ward);
	DTO_FIELD_INFO(admission_ward) {
		info->description = ZH_WORDS_GETTER("charging.admission.admission_ward");
	}

	// Attending doctor
	DTO_FIELD(String, attending_doctor);
	DTO_FIELD_INFO(attending_doctor) {
		info->description = ZH_WORDS_GETTER("charging.admission.attending_doctor");
	}

	// Admission diagnosis
	DTO_FIELD(String, admissionDiagnosis);
	DTO_FIELD_INFO(admissionDiagnosis) {
		info->description = ZH_WORDS_GETTER("charging.admission.admissionDiagnosis");
	}

	// Diagnosis description
	DTO_FIELD(String, diagnosisDesc);
	DTO_FIELD_INFO(diagnosisDesc) {
		info->description = ZH_WORDS_GETTER("charging.admission.diagnosisDesc");
	}

	// Patient condition
	DTO_FIELD(String, patientCondition);
	DTO_FIELD_INFO(patientCondition) {
		info->description = ZH_WORDS_GETTER("charging.admission.patientCondition");
	}

	// Admission type
	DTO_FIELD(String, admissionType);
	DTO_FIELD_INFO(admissionType) {
		info->description = ZH_WORDS_GETTER("charging.admission.admissionType");
	}

	// Admission mode
	DTO_FIELD(String, admissionMode);
	DTO_FIELD_INFO(admissionMode) {
		info->description = ZH_WORDS_GETTER("charging.admission.admissionMode");
	}

	// Fee nature / payment type
	DTO_FIELD(String, feeNature);
	DTO_FIELD_INFO(feeNature) {
		info->description = ZH_WORDS_GETTER("charging.admission.feeNature");
	}

	// Admission date
	DTO_FIELD(String, admissionDate);
	DTO_FIELD_INFO(admissionDate) {
		info->description = ZH_WORDS_GETTER("charging.admission.admissionDate");
	}

	// Prepay amount
	DTO_FIELD(String, prepayAmount);
	DTO_FIELD_INFO(prepayAmount) {
		info->description = ZH_WORDS_GETTER("charging.admission.prepayAmount");
	}

	// Payment method
	DTO_FIELD(String, payWay);
	DTO_FIELD_INFO(payWay) {
		info->description = ZH_WORDS_GETTER("charging.admission.payWay");
	}

	// Admission department ID (project flow primary source; falls back to admissionDept)
	DTO_FIELD(String, organizationId);
	DTO_FIELD_INFO(organizationId) {
		info->description = ZH_WORDS_GETTER("charging.admission.organizationId");
	}

	// Admission ward ID (project flow primary source; falls back to admission_ward)
	DTO_FIELD(String, wardId);
	DTO_FIELD_INFO(wardId) {
		info->description = ZH_WORDS_GETTER("charging.admission.wardId");
	}

	// Attending doctor ID (project flow primary source; falls back to attending_doctor)
	DTO_FIELD(String, attendingDoctorId);
	DTO_FIELD_INFO(attendingDoctorId) {
		info->description = ZH_WORDS_GETTER("charging.admission.attendingDoctorId");
	}

	// Contract (fee nature) ID, persisted to adm_account.contract_no
	DTO_FIELD(String, contractId);
	DTO_FIELD_INFO(contractId) {
		info->description = ZH_WORDS_GETTER("charging.admission.contractId");
	}

	// Prepayment payment details; total amount falls back to prepayAmount
	DTO_FIELD(Vector<Object<AdmissionPaymentDetailDTO>>, paymentDetails);
	DTO_FIELD_INFO(paymentDetails) {
		info->description = ZH_WORDS_GETTER("charging.admission.paymentDetails");
	}

	// Payload DTO for auth context
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:

	// Data validation
	std::string validate()
	{
		if (!age || age < 0)
			return "age invalidate.";
		if (!name || name->empty())
			return "name invalidate.";
		if (!sex || sex->empty())
			return "sex invalidate.";
		if (!idCard || idCard->empty())
			return "idCard invalidate.";
		if (!birthtime || birthtime->empty())
			return "birthtime invalidate.";
		if (!admissionDate || admissionDate->empty())
			return "admissiondata invalidate.";
		return "";
	}
};

class AdmissionDeleteDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionDeleteDTO, DTO);
	// Encounter ID to cancel
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("charging.admission.encounterId");
	}

	// Cancel reason, persisted to adm_encounter_reason
	DTO_FIELD(String, cancelReason);
	DTO_FIELD_INFO(cancelReason) {
		info->description = ZH_WORDS_GETTER("charging.admission.delete");
	}

	// Whether the medical insurance payment has been revoked (mark only, no reversal logic)
	DTO_FIELD(Boolean, insuranceRevoked);
	DTO_FIELD_INFO(insuranceRevoked) {
		info->description = ZH_WORDS_GETTER("charging.admission.insuranceRevoked");
	}

	// Payload DTO for auth context
	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	// Data validation
	std::string validate()
	{
		if (!encounterId || encounterId->empty())
			return "encounterId invalidate.";
		if (!cancelReason || cancelReason->empty())
			return "cancelReason invalidate.";
		return "";
	}
};

/**
 * Admission registration result.
 * Returned on successful POST /charging/admission/add.
 */
class AdmissionAddResultDTO : public oatpp::DTO
{
	DTO_INIT(AdmissionAddResultDTO, DTO);

	// Generated encounter ID (adm_encounter.id)
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("charging.admission.resultEncounterId");
	}

	// Generated patient ID (adm_patient.id)
	DTO_FIELD(String, patientId);
	DTO_FIELD_INFO(patientId) {
		info->description = ZH_WORDS_GETTER("charging.admission.resultPatientId");
	}

	// Generated encounter business number (adm_encounter.bus_no)
	DTO_FIELD(String, encounterBusNo);
	DTO_FIELD_INFO(encounterBusNo) {
		info->description = ZH_WORDS_GETTER("charging.admission.resultEncounterBusNo");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_ADMISSION_DTO_
