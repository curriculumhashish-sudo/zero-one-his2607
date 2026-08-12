#pragma once
#ifndef _GET_PATIENT_LIST_DTO_
#define _GET_PATIENT_LIST_DTO_
#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)
class GetPatientListDTO : public oatpp::DTO
{
	DTO_INIT(GetPatientListDTO, DTO);

	// 就诊主键，直接返回数据库主键字符串，避免转数字丢精度或变0
	DTO_FIELD(String, encounter_id);
	DTO_FIELD_INFO(encounter_id) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.encounter_id");
	}
	// 患者主键，可直接用于费用明细/费用合计/历史结算查询
	DTO_FIELD(String, patient_id);
	DTO_FIELD_INFO(patient_id) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.patient_id");
	}
	// 账户主键，正式结算时必传
	DTO_FIELD(String, account_id);
	DTO_FIELD_INFO(account_id) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.account_id");
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
	// 入院日期按日期字符串返回，不能转数字
	DTO_FIELD(String, admit_date);
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
class GetPatientListPageDTO : public PageDTO<GetPatientListDTO::Wrapper>
{
	DTO_INIT(GetPatientListPageDTO, PageDTO<GetPatientListDTO::Wrapper>);
};
#include OATPP_CODEGEN_END(DTO)
#endif