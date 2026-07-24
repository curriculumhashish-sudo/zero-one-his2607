#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: xinpingqihe
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

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include "Macros.h"
#include "ServerInfo.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(DTO)

class CostSummaryQuery : public oatpp::DTO {
	DTO_INIT(CostSummaryQuery, DTO);

	// id (used by query-one)
	API_DTO_FIELD(String, id, ZH_WORDS_GETTER("chargedetails.field.id"), false, "");
	// encounter id
	API_DTO_FIELD(String, encounterId, ZH_WORDS_GETTER("chargedetails.field.encounterId"), false, "");
	// start date
	API_DTO_FIELD(String, startDate, ZH_WORDS_GETTER("chargedetails.field.startDate"), false, "");
	// end date
	API_DTO_FIELD(String, endDate, ZH_WORDS_GETTER("chargedetails.field.endDate"), false, "");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_COSTSUMMARYQUERY_H_
