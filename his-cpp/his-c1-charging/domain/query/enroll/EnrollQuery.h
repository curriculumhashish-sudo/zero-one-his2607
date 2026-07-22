#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: QQQQ
 @Date: 2026/07/18

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
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class EnrollQuery : public PageQuery
{
	DTO_INIT(EnrollQuery, PageQuery);

	// Patient name or inpatient number.
	DTO_FIELD(String, keyword);
	DTO_FIELD_INFO(keyword)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.field.keyword");
		info->required = false;
	}

	// Patient identity card number.
	DTO_FIELD(String, idCard);
	DTO_FIELD_INFO(idCard)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.field.id_card");
		info->required = false;
	}

	// Registration statuses separated by commas.
	DTO_FIELD(String, registrationListStatuses);
	DTO_FIELD_INFO(registrationListStatuses)
	{
		info->description = ZH_WORDS_GETTER("QQQQ.field.statuses");
		info->required = false;
	}
};

#include OATPP_CODEGEN_END(DTO)
