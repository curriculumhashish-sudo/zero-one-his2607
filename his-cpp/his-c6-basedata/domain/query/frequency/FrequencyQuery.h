#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
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
#ifndef _FREQUENCYQUERY_H_
#define _FREQUENCYQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class FrequencyQuery : public PageQuery
{
	DTO_INIT(FrequencyQuery, PageQuery);

	// Filter by frequency status.
	API_DTO_FIELD_REQUIRE(String, status, ZH_WORDS_GETTER("frequency.field.status"), false);
	// Match a frequency name or code.
	API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("frequency.field.keywords"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYQUERY_H_
