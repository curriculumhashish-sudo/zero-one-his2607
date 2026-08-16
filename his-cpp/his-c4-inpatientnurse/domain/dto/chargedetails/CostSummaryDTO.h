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
#ifndef _COSTSUMMARYDTO_H_
#define _COSTSUMMARYDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class CostSummaryDTO : public oatpp::DTO {
	DTO_INIT(CostSummaryDTO, DTO);

	// type code
	API_DTO_FIELD_REQUIRE(String, typeCode, ZH_WORDS_GETTER("chargedetails.field.typeCode"), true);
	// type name
	API_DTO_FIELD_REQUIRE(String, typeName, ZH_WORDS_GETTER("chargedetails.field.typeName"), true);
	// total amount
	API_DTO_FIELD_REQUIRE(Float64, totalAmount, ZH_WORDS_GETTER("chargedetails.field.totalAmount"), true);
	// count
	API_DTO_FIELD_REQUIRE(Int64, count, ZH_WORDS_GETTER("chargedetails.field.count"), true);
	// summary date
	API_DTO_FIELD_REQUIRE(String, summaryDate, ZH_WORDS_GETTER("chargedetails.field.summaryDate"), true);

public:
	CostSummaryDTO()
	{
		totalAmount = 0.0;
		count = static_cast<v_int64>(0);
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTSUMMARYDTO_H_
