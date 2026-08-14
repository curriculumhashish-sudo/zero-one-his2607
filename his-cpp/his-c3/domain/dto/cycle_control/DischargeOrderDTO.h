#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 10:59:38

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
#ifndef _DISCHARGE_ORDER_DTO_H_
#define _DISCHARGE_ORDER_DTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 创建出院医嘱请求DTO。
 * 仅需就诊ID即可发起出院。
 */
class CreateDischargeOrderDTO : public oatpp::DTO
{
	DTO_INIT(CreateDischargeOrderDTO, DTO);
	// 就诊ID（必填）
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("inpdocws.field.encounterId"), true);

	CC_SYNTHESIZE(const PayloadDTO*, _payload, Payload);
public:
	std::string validate() {
		if (!encounterId || encounterId->empty())
			return "encounterId is required.";
		return "";
	}
};

/**
 * 出院医嘱响应DTO，包含创建后的出院医嘱详情。
 */
class DischargeOrderDetailDTO : public oatpp::DTO
{
	DTO_INIT(DischargeOrderDetailDTO, DTO);
	// 出院医嘱ID（wkf_service_request.id）
	DTO_FIELD(String, orderId);
	DTO_FIELD_INFO(orderId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.orderId");
	}
	// 就诊ID
	DTO_FIELD(String, encounterId);
	DTO_FIELD_INFO(encounterId) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.encounterId");
	}
	// 医嘱状态（wkf_service_request.status_enum）
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.orderStatus");
	}
	// 就诊新状态（创建出院医嘱后 encounter.status_enum 变为 ON_HOLD）
	DTO_FIELD(String, encounterNewStatus);
	DTO_FIELD_INFO(encounterNewStatus) {
		info->description = ZH_WORDS_GETTER("inpdocws.field.encounterNewStatus");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DISCHARGE_ORDER_DTO_H_
