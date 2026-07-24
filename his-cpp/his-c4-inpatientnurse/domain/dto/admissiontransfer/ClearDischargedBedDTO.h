#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/21 00:00:00

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
#ifndef _CLEAR_DISCHARGED_BED_DTO_
#define _CLEAR_DISCHARGED_BED_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class ClearDischargedBedDTO : public oatpp::DTO
{
	DTO_INIT(ClearDischargedBedDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);

public:
	ClearDischargedBedDTO()
	{
		encounterId = "337907884548657152";
	}
};

class ClearDischargedBedResponseDTO : public oatpp::DTO
{
	DTO_INIT(ClearDischargedBedResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId,
		ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);
	API_DTO_FIELD_REQUIRE(Boolean, succeeded,
		ZH_WORDS_GETTER("admissiontransfer.field.succeeded"), true);

public:
	ClearDischargedBedResponseDTO()
	{
		encounterId = "337907884548657152";
		succeeded = true;
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_CLEAR_DISCHARGED_BED_DTO_
