#pragma once

#ifndef _SIGNMEDICALADVICEDTO_H_
#define _SIGNMEDICALADVICEDTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

// 批量签发医嘱的请求体。
class SignMedicalAdviceDTO : public oatpp::DTO
{
	DTO_INIT(SignMedicalAdviceDTO, DTO);
	// 待签发医嘱记录的标识列表。
	API_DTO_FIELD_REQUIRE(List<String>, medicalAdviceIds,
		ZH_WORDS_GETTER("medical-advice.field.medical-advice-ids"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SIGNMEDICALADVICEDTO_H_
