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
#ifndef _COSTDETAILCONTROLLER_H_
#define _COSTDETAILCONTROLLER_H_

#include "ApiHelper.h"
#include "domain/query/chargedetails/CostDetailQuery.h"
#include "domain/query/chargedetails/CostSummaryQuery.h"
#include "domain/vo/chargedetails/CostDetailVO.h"
#include "domain/vo/chargedetails/CostSummaryVO.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("chargedetails.tag")

class CostDetailController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(CostDetailController);
public:
	// 1. Paginated query for cost details
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("chargedetails.query-all.summary"),
		queryAll, CostDetailQuery, CostDetailPageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chargedetails/query-all",
		queryAll, CostDetailQuery, execQueryAll(query));

	// 2. Query single cost detail by ID
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chargedetails.query-one.summary"),
		queryOne, CostDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id",
			ZH_WORDS_GETTER("chargedetails.field.id"), "1", true););
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chargedetails/query-one",
		queryOne, CostSummaryQuery, execQueryOne(query));

	// 3. Cost summary by encounter
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chargedetails.summary-by-encounter.summary"),
		summaryByEncounter, CostSummaryListJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
			ZH_WORDS_GETTER("chargedetails.field.encounterId"), "E123", true););
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chargedetails/summary-by-encounter",
		summaryByEncounter, CostSummaryQuery, execSummaryByEncounter(query));

	// 4. Cost summary by date range
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chargedetails.summary-by-date.summary"),
		summaryByDate, CostSummaryListJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "startDate",
			ZH_WORDS_GETTER("chargedetails.field.startDate"), "2024-01-01", true);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate",
			ZH_WORDS_GETTER("chargedetails.field.endDate"), "2024-12-31", true);
		API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
			ZH_WORDS_GETTER("chargedetails.field.encounterId"), "E123", false););
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chargedetails/summary-by-date",
		summaryByDate, CostSummaryQuery, execSummaryByDate(query));

private:
	CostDetailPageJsonVO::Wrapper execQueryAll(const CostDetailQuery::Wrapper& query);
	CostDetailJsonVO::Wrapper execQueryOne(const CostSummaryQuery::Wrapper& query);
	CostSummaryListJsonVO::Wrapper execSummaryByEncounter(const CostSummaryQuery::Wrapper& query);
	CostSummaryListJsonVO::Wrapper execSummaryByDate(const CostSummaryQuery::Wrapper& query);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_COSTDETAILCONTROLLER_H_
