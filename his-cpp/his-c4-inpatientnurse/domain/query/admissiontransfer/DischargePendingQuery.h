#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/08/06 00:00:00

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
#ifndef _DISCHARGE_PENDING_QUERY_
#define _DISCHARGE_PENDING_QUERY_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DischargePendingQuery : public oatpp::DTO
{
	DTO_INIT(DischargePendingQuery, DTO);

	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("admissiontransfer.field.encounterId"), true);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DISCHARGE_PENDING_QUERY_
