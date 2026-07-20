#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: his-c4
 @Date: 2026/07/18 22:00:00

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
#ifndef _DISPENSESUMMARY_QUERY_
#define _DISPENSESUMMARY_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Page query params of the summary drug-collection bill list.
 * Filter dimensions defined by the business flow chart: bill status + date.
 */
class DispenseSummaryQuery : public PageQuery
{
	DTO_INIT(DispenseSummaryQuery, PageQuery);
	// bill status enum (WAIT_REVIEW / DISPENSED); optional filter, so NO runtime default
	// (swagger doc default is provided by API_DEF_ADD_QUERY_PARAMS in the controller)
	DTO_FIELD(String, statusEnum);
	DTO_FIELD_INFO(statusEnum) {
		info->description = ZH_WORDS_GETTER("dispense.field.status-enum");
	}
	// dispense date filter (yyyy-MM-dd); optional filter, so NO runtime default
	DTO_FIELD(String, dispenseDate);
	DTO_FIELD_INFO(dispenseDate) {
		info->description = ZH_WORDS_GETTER("dispense.field.dispense-date");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DISPENSESUMMARY_QUERY_
