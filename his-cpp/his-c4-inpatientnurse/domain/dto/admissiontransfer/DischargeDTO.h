#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
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
#ifndef _DISCHARGE_DTO_
#define _DISCHARGE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DischargeDTO : public oatpp::DTO
{
	DTO_INIT(DischargeDTO, DTO);

	API_DTO_FIELD(String, encounterId,
		ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true,
		"337907884548657152");
};

class DischargeResultDTO : public oatpp::DTO
{
	DTO_INIT(DischargeResultDTO, DTO);

	API_DTO_FIELD(String, encounterId,
		ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true,
		"337907884548657152");
	API_DTO_FIELD(Boolean, succeeded,
		ZH_WORDS_GETTER("admissiontransfer.field.succeeded"), true, true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DISCHARGE_DTO_
