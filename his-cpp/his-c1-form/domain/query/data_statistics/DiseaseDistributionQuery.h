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
#ifndef _DISEASE_DISTRIBUTION_QUERY_
#define _DISEASE_DISTRIBUTION_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class DiseaseDistributionQuery : public PageQuery
{
	DTO_INIT(DiseaseDistributionQuery, PageQuery);

	// start date (ISO format, optional: if empty, query all or current month)
	DTO_FIELD(String, startDate);
	DTO_FIELD_INFO(startDate) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.start_date");
	}

	// end date (ISO format, optional)
	DTO_FIELD(String, endDate);
	DTO_FIELD_INFO(endDate) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.end_date");
	}

	// organization id (optional, for data isolation)
	DTO_FIELD(String, orgId);
	DTO_FIELD_INFO(orgId) {
		info->description = ZH_WORDS_GETTER("W.form.data_statistics.field.org_id");
	}
};

#include OATPP_CODEGEN_END(DTO)
#endif
