#pragma once

#ifndef _GET_YXY_LIST_DTO_
#define _GET_YXY_LIST_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class GetExpenseDetailsListDTO : public oatpp::DTO
{
	DTO_INIT(GetExpenseDetailsListDTO, DTO);

	//项目名称
	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.name");
	}

	//费用类别
	DTO_FIELD(String, cost_type);
	DTO_FIELD_INFO(cost_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.cost_type");
	}

	//单价
	DTO_FIELD(Float64, unit);
	DTO_FIELD_INFO(unit) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.unit");
	}

	//数量
	DTO_FIELD(UInt32, num);
	DTO_FIELD_INFO(num) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.num");
	}

	//金额
	DTO_FIELD(Float64, amount);
	DTO_FIELD_INFO(amount) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.amount");
	}

	//医保属性
	DTO_FIELD(Float64, medicial_type);
	DTO_FIELD_INFO(medicial_type) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.medicial_type");
	}

};

#include OATPP_CODEGEN_END(DTO)
#endif