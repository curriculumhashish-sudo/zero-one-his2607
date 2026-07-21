#pragma once

#ifndef _GET_TOTAL_FEES_DTO_
#define _GET_TOTAL_FEES_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetTotalFeesDTO : public oatpp::DTO
{
	DTO_INIT(GetTotalFeesDTO, DTO);

	//住院总费用
	DTO_FIELD(Float64, total_hospital_fee);
	DTO_FIELD_INFO(total_hospital_fee) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.total_hospital_fee");
	}

	//个人自付金额
	DTO_FIELD(Float64, personal_pay_fee);
	DTO_FIELD_INFO(personal_pay_fee) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.personal_pay_fee");
	}

	//预交押金总额
	DTO_FIELD(Float64, prepay_total_fee);
	DTO_FIELD_INFO(prepay_total_fee) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.prepay_total_fee");
	}

	//应补/应退金额
	DTO_FIELD(Float64, final_balance_fee);
	DTO_FIELD_INFO(final_balance_fee) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.final_balance_fee");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif