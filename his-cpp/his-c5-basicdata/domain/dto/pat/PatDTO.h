#pragma once

#ifndef _PAT_DTO_H_
#define _PAT_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 患者列表单行数据DTO
 */
class PatDTO : public oatpp::DTO
{
	DTO_INIT(PatDTO, DTO);

	DTO_FIELD(Int64, id);
	DTO_FIELD_INFO(id) {
		info->description = ZH_WORDS_GETTER("pat.dto.field.id");
	}

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("pat.dto.field.name");
	}

	DTO_FIELD(String, idCard);
	DTO_FIELD_INFO(idCard) {
		info->description = ZH_WORDS_GETTER("pat.dto.field.idCard");
	}

	DTO_FIELD(Int32, gender);
	DTO_FIELD_INFO(gender) {
		info->description = ZH_WORDS_GETTER("pat.dto.field.gender");
	}

	DTO_FIELD(Int32, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("pat.dto.field.age");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // _PAT_DTO_H_