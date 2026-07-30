#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
#ifndef _COSTSUMMARYQUERY_H_
#define _COSTSUMMARYQUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostSummaryQuery : public oatpp::DTO {
	DTO_INIT(CostSummaryQuery, DTO);

	// encounter id
	API_DTO_FIELD_REQUIRE(String, encounterId, ZH_WORDS_GETTER("chargedetails.field.encounterId"), false);
	// start date
	API_DTO_FIELD_REQUIRE(String, startDate, ZH_WORDS_GETTER("chargedetails.field.startDate"), false);
	// end date
	API_DTO_FIELD_REQUIRE(String, endDate, ZH_WORDS_GETTER("chargedetails.field.endDate"), false);

public:
	CostSummaryQuery()
	{
		encounterId = "337907884548657152";
		startDate = "2026-07-01";
		endDate = "2026-07-24";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTSUMMARYQUERY_H_
