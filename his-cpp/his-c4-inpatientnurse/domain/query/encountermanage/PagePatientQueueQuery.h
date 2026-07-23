#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/23 00:00:00

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
#ifndef _PAGE_PATIENT_QUEUE_QUERY_
#define _PAGE_PATIENT_QUEUE_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class PagePatientQueueQuery : public PageQuery
{
	DTO_INIT(PagePatientQueueQuery, PageQuery);

	API_DTO_FIELD_REQUIRE(String, keyword,
		ZH_WORDS_GETTER("encountermanage.field.keyword"), false);
	API_DTO_FIELD_REQUIRE(String, wardId,
		ZH_WORDS_GETTER("encountermanage.field.wardId"), false);
	API_DTO_FIELD_REQUIRE(String, houseId,
		ZH_WORDS_GETTER("encountermanage.field.houseId"), false);
	API_DTO_FIELD_REQUIRE(String, nurseStationExecutionDate,
		ZH_WORDS_GETTER("encountermanage.field.nurseStationExecutionDate"), false);
	API_DTO_FIELD_REQUIRE(String, nurseStationTaskFilter,
		ZH_WORDS_GETTER("encountermanage.field.nurseStationTaskFilter"), false);
	API_DTO_FIELD_REQUIRE(String, nurseStationStatusFilter,
		ZH_WORDS_GETTER("encountermanage.field.nurseStationStatusFilter"), false);
	API_DTO_FIELD_REQUIRE(String, startDate,
		ZH_WORDS_GETTER("encountermanage.field.startDate"), false);
	API_DTO_FIELD_REQUIRE(String, endDate,
		ZH_WORDS_GETTER("encountermanage.field.endDate"), false);
	API_DTO_FIELD_REQUIRE(String, sortBy,
		ZH_WORDS_GETTER("encountermanage.field.sortBy"), false);
	API_DTO_FIELD_REQUIRE(String, sortDirection,
		ZH_WORDS_GETTER("encountermanage.field.sortDirection"), false);
	API_DTO_FIELD_REQUIRE(String, where,
		ZH_WORDS_GETTER("encountermanage.field.where"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_PAGE_PATIENT_QUEUE_QUERY_
