#pragma once

#ifndef _SETTLE_DTO_
#define _SETTLE_DTO_
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SettleDTO : public oatpp::DTO
{
	DTO_INIT(SettleDTO, DTO);

	DTO_FIELD(String, hosp_id);
	DTO_FIELD_INFO(hosp_id) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.hosp_id");
	}

	//结算时间
	DTO_FIELD(UInt64, time);
	DTO_FIELD_INFO(time) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.time");
	}

	//结算类型
	DTO_FIELD(String, type);
	DTO_FIELD_INFO(type) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.type");
	}

	//结算金额
	DTO_FIELD(Float64, total_amount);
	DTO_FIELD_INFO(total_amount) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.total_amount");
	}

	//医保报销
	DTO_FIELD(Float64, reimburse);
	DTO_FIELD_INFO(reimburse) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.reimburse");
	}

	//个人自付
	DTO_FIELD(Float64, self_cost);
	DTO_FIELD_INFO(self_cost) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.self_cost");
	}

	//结算方法
	DTO_FIELD(String, method);
	DTO_FIELD_INFO(method) {
		info->description = ZH_WORDS_GETTER("yxy.charging.settle.method");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif