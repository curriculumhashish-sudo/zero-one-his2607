#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: byj
 @Date: 2026/08/02

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class Pay_ManageGetPatientPayRecordsDTO : public oatpp::DTO
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
