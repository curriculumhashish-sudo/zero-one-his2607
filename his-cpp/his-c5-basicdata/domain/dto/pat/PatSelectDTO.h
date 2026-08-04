#pragma once
#ifndef _PAT_SELECT_DTO_H_
#define _PAT_SELECT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * One row in select-patient popup table.
 */
class PatSelectDTO : public oatpp::DTO
{
	DTO_INIT(PatSelectDTO, DTO);

	// Medical record number (bus_no)
	DTO_FIELD(String, busNo);
	DTO_FIELD_INFO(busNo) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.medicalNo");
	}

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.name");
	}

	// Display gender text, e.g. 男性 / 女性
	DTO_FIELD(String, gender);
	DTO_FIELD_INFO(gender) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.gender");
	}

	DTO_FIELD(String, birthDate);
	DTO_FIELD_INFO(birthDate) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.birthDay");
	}

	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.age");
	}

	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.phone");
	}

	DTO_FIELD(String, idCard);
	DTO_FIELD_INFO(idCard) {
		info->description = ZH_WORDS_GETTER("pat.select.dto.field.idCard");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_SELECT_DTO_H_