#pragma once

#ifndef _WITHDRAWMEDICALADVICEDTO_H_
#define _WITHDRAWMEDICALADVICEDTO_H_

#include "../../GlobalInclude.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 批量撤回医嘱请求DTO
 */
class WithdrawMedicalAdviceDTO : public oatpp::DTO
{
	DTO_INIT(WithdrawMedicalAdviceDTO, DTO);
	// 医嘱ID列表
	API_DTO_FIELD_REQUIRE(List<String>, medicalAdviceIds,
		ZH_WORDS_GETTER("medical-advice.field.medical-advice-ids"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_WITHDRAWMEDICALADVICEDTO_H_
