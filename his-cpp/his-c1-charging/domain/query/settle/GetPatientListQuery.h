#pragma once

#ifndef _GET_PATIENT_LIST_QUERY_
#define _GET_PATIENT_LIST_QUERY_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetPatientListQuery : public PageQuery
{
	DTO_INIT(GetPatientListQuery, PageQuery);



	DTO_FIELD(UInt64, encounter_id);
	DTO_FIELD_INFO(encounter_id) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.encounter_id");
	}

	DTO_FIELD(String, inpatient_no);
	DTO_FIELD_INFO(inpatient_no) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.inpatient_no");
	}

	DTO_FIELD(String, patient_name);
	DTO_FIELD_INFO(patient_name) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.patient_name");
	}

	DTO_FIELD(UInt8, gender);
	DTO_FIELD_INFO(gender) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.gender");
	}

	DTO_FIELD(UInt32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.age");
	}

	DTO_FIELD(String, dept_name);
	DTO_FIELD_INFO(dept_name) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.dept_name");
	}

	DTO_FIELD(String, ward_name);
	DTO_FIELD_INFO(ward_name) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.ward_name");
	}

	DTO_FIELD(String, bed_no);
	DTO_FIELD_INFO(bed_no) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.bed_no");
	}

	DTO_FIELD(UInt64, admit_date);
	DTO_FIELD_INFO(admit_date) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.admit_date");
	}

	DTO_FIELD(UInt8, insurance_type);
	DTO_FIELD_INFO(insurance_type) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.insurance_type");
	}

	
	DTO_FIELD(String, doctor_name);
	DTO_FIELD_INFO(doctor_name) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.doctor_name");
	}

	
};

#include OATPP_CODEGEN_END(DTO)
#endif