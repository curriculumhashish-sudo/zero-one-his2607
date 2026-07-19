#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: QQQQ
 @Date: 2026/07/18

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

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Inpatient registration list item.
 */
class InpatientRegistrationListDTO : public oatpp::DTO
{
	DTO_INIT(InpatientRegistrationListDTO, DTO);

	// Inpatient registration record ID.
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.encounter_id");
	}

	// Inpatient number.
	DTO_FIELD(String, encounterBusNo);
	DTO_FIELD_INFO(encounterBusNo)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.encounter_bus_no");
	}

	// Patient name.
	DTO_FIELD(String, patientName);
	DTO_FIELD_INFO(patientName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.patient_name");
	}

	// Registration status code.
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.status_enum");
	}

	// Registration status name.
	DTO_FIELD(String, statusEnumText);
	DTO_FIELD_INFO(statusEnumText)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.status_enum_text");
	}

	// Department name.
	DTO_FIELD(String, organizationName);
	DTO_FIELD_INFO(organizationName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.organization_name");
	}

	// Ward name.
	DTO_FIELD(String, wardName);
	DTO_FIELD_INFO(wardName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.ward_name");
	}

	// Bed name.
	DTO_FIELD(String, bedName);
	DTO_FIELD_INFO(bedName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.bed_name");
	}

	// Admission time.
	DTO_FIELD(String, startTime);
	DTO_FIELD_INFO(startTime)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.start_time");
	}
};

/**
 * Paginated inpatient registration data.
 */
class InpatientRegistrationPageDTO : public PageDTO<InpatientRegistrationListDTO::Wrapper>
{
	DTO_INIT(InpatientRegistrationPageDTO, PageDTO<InpatientRegistrationListDTO::Wrapper>);
};

/**
 * Inpatient registration detail.
 */
class InpatientRegistrationDetailDTO : public InpatientRegistrationListDTO
{
	DTO_INIT(InpatientRegistrationDetailDTO, InpatientRegistrationListDTO);

	// Patient ID.
	DTO_FIELD(String, patientId);
	DTO_FIELD_INFO(patientId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.patient_id");
	}

	// Patient identity card number.
	DTO_FIELD(String, idCard);
	DTO_FIELD_INFO(idCard)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.id_card");
	}

	// Gender code.
	DTO_FIELD(String, genderEnum);
	DTO_FIELD_INFO(genderEnum)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.gender_enum");
	}

	// Gender name.
	DTO_FIELD(String, genderEnumText);
	DTO_FIELD_INFO(genderEnumText)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.gender_enum_text");
	}

	// Birth date.
	DTO_FIELD(String, birthDate);
	DTO_FIELD_INFO(birthDate)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.birth_date");
	}

	// Patient age.
	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.age");
	}

	// Patient phone number.
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.phone");
	}

	// Number of hospital admissions.
	DTO_FIELD(Int32, admissionCount);
	DTO_FIELD_INFO(admissionCount)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.admission_count");
	}

	// Department ID.
	DTO_FIELD(String, organizationId);
	DTO_FIELD_INFO(organizationId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.organization_id");
	}

	// Ward ID.
	DTO_FIELD(String, wardId);
	DTO_FIELD_INFO(wardId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.ward_id");
	}

	// Bed ID.
	DTO_FIELD(String, bedId);
	DTO_FIELD_INFO(bedId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.bed_id");
	}

	// Outpatient doctor name.
	DTO_FIELD(String, outpatientDoctorName);
	DTO_FIELD_INFO(outpatientDoctorName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.outpatient_doctor_name");
	}

	// Attending doctor ID.
	DTO_FIELD(String, attendingDoctorId);
	DTO_FIELD_INFO(attendingDoctorId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.attending_doctor_id");
	}

	// Attending doctor name.
	DTO_FIELD(String, attendingDoctorName);
	DTO_FIELD_INFO(attendingDoctorName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.attending_doctor_name");
	}

	// Admission diagnosis ID.
	DTO_FIELD(String, conditionDefinitionId);
	DTO_FIELD_INFO(conditionDefinitionId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.condition_definition_id");
	}

	// Admission diagnosis name.
	DTO_FIELD(String, conditionDefinitionName);
	DTO_FIELD_INFO(conditionDefinitionName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.condition_definition_name");
	}

	// Diagnosis description.
	DTO_FIELD(String, diagnosisDescription);
	DTO_FIELD_INFO(diagnosisDescription)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.diagnosis_description");
	}

	// Admission source code.
	DTO_FIELD(String, admitSourceCode);
	DTO_FIELD_INFO(admitSourceCode)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.admit_source_code");
	}

	// Admission source name.
	DTO_FIELD(String, admitSourceCodeText);
	DTO_FIELD_INFO(admitSourceCodeText)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.admit_source_code_text");
	}

	// Admission method code.
	DTO_FIELD(String, inWayCode);
	DTO_FIELD_INFO(inWayCode)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.in_way_code");
	}

	// Admission method name.
	DTO_FIELD(String, inWayCodeText);
	DTO_FIELD_INFO(inWayCodeText)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.in_way_code_text");
	}

	// Patient condition code.
	DTO_FIELD(String, priorityEnum);
	DTO_FIELD_INFO(priorityEnum)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.priority_enum");
	}

	// Patient condition name.
	DTO_FIELD(String, priorityEnumText);
	DTO_FIELD_INFO(priorityEnumText)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.priority_enum_text");
	}

	// Contract ID.
	DTO_FIELD(String, contractId);
	DTO_FIELD_INFO(contractId)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.contract_id");
	}

	// Contract name.
	DTO_FIELD(String, contractName);
	DTO_FIELD_INFO(contractName)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.charging.hospitalization_registration.field.contract_name");
	}
};

#include OATPP_CODEGEN_END(DTO)
