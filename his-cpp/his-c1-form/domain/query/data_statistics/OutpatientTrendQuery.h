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
#ifndef _OUTPATIENT_TREND_QUERY_
#define _OUTPATIENT_TREND_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class OutpatientTrendQuery : public PageQuery
{
	DTO_INIT(OutpatientTrendQuery, PageQuery);

	// start date (format: YYYY-MM-DD)
	DTO_FIELD(String, dateFrom);
	DTO_FIELD_INFO(dateFrom) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.date_from");
	}

	// end date (format: YYYY-MM-DD)
	DTO_FIELD(String, dateTo);
	DTO_FIELD_INFO(dateTo) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.date_to");
	}

	// granularity: day / month / year
	DTO_FIELD(String, granularity);
	DTO_FIELD_INFO(granularity) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.granularity");
	}

	// organization id (optional, for data isolation)
	DTO_FIELD(String, organizationId);
	DTO_FIELD_INFO(organizationId) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.organization_id");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
