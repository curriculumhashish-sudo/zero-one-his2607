#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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

	API_DTO_FIELD_REQUIRE(String, statusEnum,
		ZH_WORDS_GETTER("dispense.field.status-enum"), false);
	API_DTO_FIELD_REQUIRE(String, dispenseDate,
		ZH_WORDS_GETTER("dispense.field.dispense-date"), false);
	API_DTO_FIELD_REQUIRE(String, destinationLocationId,
		ZH_WORDS_GETTER("dispense.field.des-location-id"), false);
	API_DTO_FIELD_REQUIRE(String, startDate,
		ZH_WORDS_GETTER("dispense.field.start-date"), false);
	API_DTO_FIELD_REQUIRE(String, endDate,
		ZH_WORDS_GETTER("dispense.field.end-date"), false);
	API_DTO_FIELD_REQUIRE(String, keyword,
		ZH_WORDS_GETTER("dispense.field.keyword"), false);
};

/**
 * Query parameters for a dispense summary detail request.
 */
class DispenseDetailQuery : public oatpp::DTO
{
	DTO_INIT(DispenseDetailQuery, DTO);

	API_DTO_FIELD_REQUIRE(String, summaryNo,
		ZH_WORDS_GETTER("dispense.field.summary-no"), true);
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DISPENSESUMMARY_QUERY_
