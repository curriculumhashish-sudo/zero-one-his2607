#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

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
#ifndef _VERIFICATION_DTO_
#define _VERIFICATION_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class VerificationWriteItemDTO : public oatpp::DTO
{
	DTO_INIT(VerificationWriteItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, orderType,
		ZH_WORDS_GETTER("medicalOrderVerification.field.orderType"), true);
	API_DTO_FIELD_REQUIRE(String, requestId,
		ZH_WORDS_GETTER("medicalOrderVerification.field.requestId"), true);
};

class VerifyPassRequestDTO : public oatpp::DTO
{
	DTO_INIT(VerifyPassRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<VerificationWriteItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medicalOrderVerification.field.items"), true);

public:
	VerifyPassRequestDTO()
	{
		items = List<VerificationWriteItemDTO::Wrapper>::createShared();
		auto item = VerificationWriteItemDTO::createShared();
		item->orderType = "MED_REQUEST";
		item->requestId = "338131147144708096";
		items->push_back(item);
	}
};

class VerifyPassResponseDTO : public oatpp::DTO
{
	DTO_INIT(VerifyPassResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("medicalOrderVerification.field.success"), false);
	API_DTO_FIELD_REQUIRE(Int32, processedCount,
		ZH_WORDS_GETTER("medicalOrderVerification.field.processedCount"), false);

public:
	VerifyPassResponseDTO()
	{
		success = true;
		processedCount = 1;
	}
};

class RejectOrdersRequestDTO : public oatpp::DTO
{
	DTO_INIT(RejectOrdersRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<VerificationWriteItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medicalOrderVerification.field.items"), true);
	API_DTO_FIELD_REQUIRE(String, reason,
		ZH_WORDS_GETTER("medicalOrderVerification.field.reason"), false);

public:
	RejectOrdersRequestDTO()
	{
		items = List<VerificationWriteItemDTO::Wrapper>::createShared();
		auto item = VerificationWriteItemDTO::createShared();
		item->orderType = "MED_REQUEST";
		item->requestId = "338131147144708096";
		items->push_back(item);
		reason = "Insufficient patient information";
	}
};

class RejectOrdersResponseDTO : public oatpp::DTO
{
	DTO_INIT(RejectOrdersResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(Boolean, success,
		ZH_WORDS_GETTER("medicalOrderVerification.field.success"), false);
	API_DTO_FIELD_REQUIRE(Int32, processedCount,
		ZH_WORDS_GETTER("medicalOrderVerification.field.processedCount"), false);

public:
	RejectOrdersResponseDTO()
	{
		success = true;
		processedCount = 1;
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_VERIFICATION_DTO_
