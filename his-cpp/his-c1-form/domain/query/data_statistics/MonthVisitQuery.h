#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: WYS
 @Date: 2026/07/20 00:00:00

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
#ifndef _MONTH_VISIT_QUERY_
#define _MONTH_VISIT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)


class MonthVisitQuery : public PageQuery
{
	DTO_INIT(MonthVisitQuery, PageQuery);

	// Start date
	DTO_FIELD(String, startDate);
	DTO_FIELD_INFO(startDate) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.start_date");
	}

	// End date
	DTO_FIELD(String, endDate);
	DTO_FIELD_INFO(endDate) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.end_date");
	}

	// Organization ID
	DTO_FIELD(UInt32, orgId);
	DTO_FIELD_INFO(orgId) {
		info->description = ZH_WORDS_GETTER("Lin.form.data_statistics.field.org_id");
	}

};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MONTH_VISIT_QUERY_
