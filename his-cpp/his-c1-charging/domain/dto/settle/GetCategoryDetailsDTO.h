#pragma once

#ifndef _GET_CATEGORY_DETAILS_DTO_
#define _GET_CATEGORY_DETAILS_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetCategoryDetailsDTO : public oatpp::DTO
{
	DTO_INIT(GetCategoryDetailsDTO, DTO);


	DTO_FIELD(UInt32, sort_num);
	DTO_FIELD_INFO(sort_num) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.sort_num");
	}


	DTO_FIELD(String, charge_type_name);
	DTO_FIELD_INFO(charge_type_name) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.charge_type_name");
	}


	DTO_FIELD(Float64, type_total_fee);
	DTO_FIELD_INFO(type_total_fee) {
		info->description = ZH_WORDS_GETTER("qi.charging.settle.type_total_fee");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif