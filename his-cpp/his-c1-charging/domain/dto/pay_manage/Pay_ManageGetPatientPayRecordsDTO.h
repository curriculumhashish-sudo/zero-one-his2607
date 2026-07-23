#pragma once
#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientPayRecordsDTO :public oatpp::DTO
{
	DTO_INIT(Pay_ManageGetPatientPayRecordsDTO, DTO);

	DTO_FIELD(String, pay_time);
	DTO_FIELD_INFO(pay_time) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.pay_time");
	}

	DTO_FIELD(String, service_type);
	DTO_FIELD_INFO(service_type) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.service_type");
	}

	DTO_FIELD(Float64, cost);
	DTO_FIELD_INFO(cost) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.cost");
	}

	DTO_FIELD(String, pay_type);
	DTO_FIELD_INFO(pay_type) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.pay_type");
	}

	DTO_FIELD(String, operateman);
	DTO_FIELD_INFO(operateman) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.operateman");
	}

	DTO_FIELD(String, bill_number);
	DTO_FIELD_INFO(bill_number) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.bill_number");
	}

	DTO_FIELD(String, status);
	DTO_FIELD_INFO(status) {
		info->description = ZH_WORDS_GETTER("byj.charging.pay_manage.status");
	}

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
};

class Pay_ManageGetPatientPayRecordsPageDTO : public PageDTO<Pay_ManageGetPatientPayRecordsDTO::Wrapper>
{
	DTO_INIT(Pay_ManageGetPatientPayRecordsPageDTO, PageDTO<Pay_ManageGetPatientPayRecordsDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)
