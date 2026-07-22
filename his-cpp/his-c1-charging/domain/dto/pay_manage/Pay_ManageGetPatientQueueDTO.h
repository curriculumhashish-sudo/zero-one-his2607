#pragma once
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientQueueDTO :public oatpp::DTO
{
	DTO_INIT(Pay_ManageGetPatientQueueDTO, DTO);

	DTO_FIELD(String, name);
	DTO_FIELD_INFO(name) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.name");
	}

	DTO_FIELD(String, sex);
	DTO_FIELD_INFO(sex) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.sex");
	}

	DTO_FIELD(UInt64, age);
	DTO_FIELD_INFO(age) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.age");
	}

	DTO_FIELD(String, hospitalized_number);
	DTO_FIELD_INFO(hospitalized_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_number");
	}

	DTO_FIELD(String, hospitalized_time);
	DTO_FIELD_INFO(hospitalized_time) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.hospitalized_time");
	}

	DTO_FIELD(String, depart);
	DTO_FIELD_INFO(depart) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.depart");
	}

	DTO_FIELD(String, bed_number);
	DTO_FIELD_INFO(bed_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.bed_number");
	}

	DTO_FIELD(String, cost_attr);
	DTO_FIELD_INFO(cost_attr) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.cost_attr");
	}

	DTO_FIELD(Float64, rest);
	DTO_FIELD_INFO(rest) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.rest");
	}

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

class Pay_ManageGetPatientQueuePageDTO : public PageDTO<Pay_ManageGetPatientQueueDTO::Wrapper>
{
	DTO_INIT(Pay_ManageGetPatientQueuePageDTO, PageDTO<Pay_ManageGetPatientQueueDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
