#pragma once
#ifndef _PATIENT_DTO_
#define _PATIENT_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者新增DTO
 */
class PatientAddDTO : public oatpp::DTO {
	DTO_INIT(PatientAddDTO, DTO);
	// 姓名
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) { info->description = ZH_WORDS_GETTER("pat.field.name"); }
	// 年龄
	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age) { info->description = ZH_WORDS_GETTER("pat.field.age"); }
	// 性别
	DTO_FIELD(String, sex);
	DTO_FIELD_INFO(sex) { info->description = ZH_WORDS_GETTER("pat.field.sex"); }
};

/**
 * 患者DTO（含ID）
 */
class PatientDTO : public PatientAddDTO {
	DTO_INIT(PatientDTO, PatientAddDTO);
	// 编号
	DTO_FIELD(String, id);
	DTO_FIELD_INFO(id) { info->description = ZH_WORDS_GETTER("pat.field.id"); }
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PATIENT_DTO_