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
#ifndef _COSTDETAILCONTROLLER_H_
#define _COSTDETAILCONTROLLER_H_

#include "ApiHelper.h"
#include "domain/query/chargedetails/CostDetailQuery.h"
#include "domain/query/chargedetails/CostSummaryQuery.h"
#include "domain/vo/chargedetails/CostDetailVO.h"
#include "domain/vo/chargedetails/CostSummaryVO.h"
#include "controller/RequestValidation.h"
using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("chargedetails.tag")

class CostDetailController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(CostDetailController);
public:
	// Query cost details by conditions.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chargedetails.query-details.summary"),
		queryAll, CostDetailPageJsonVO::Wrapper, API_TAG,
		API_DEF_QUERY_PARAM_BUILD(CostDetailQuery);
		info->queryParams["patientId"].addExample("default", String("326940849401761792"));
		info->queryParams["encounterId"].addExample("default", String("337907884548657152"));
		info->queryParams["busNo"].addExample("default", String("CHG202607240001"));
		info->queryParams["typeCode"].addExample("default", String("MEDICATION"));
		info->queryParams["startDate"].addExample("default", String("2026-07-01"));
		info->queryParams["endDate"].addExample("default", String("2026-07-24"));
		info->responses[Status::CODE_200].addExample(
			"success", CostDetailPageJsonVO::createShared());
	)
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chargedetails/query-details",
		queryAll, CostDetailQuery, execQueryAll(query, authObject->getPayload()));

	// Query cost summary by conditions.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("chargedetails.query-summary.summary"),
		summaryByEncounter, CostSummaryListJsonVO::Wrapper, API_TAG,
		API_DEF_QUERY_PARAM_BUILD(CostSummaryQuery);
		info->queryParams["patientId"].addExample("default", String("326940849401761792"));
		info->queryParams["encounterId"].addExample("default", String("337907884548657152"));
		info->queryParams["startDate"].addExample("default", String("2026-07-01"));
		info->queryParams["endDate"].addExample("default", String("2026-07-24"));
		info->responses[Status::CODE_200].addExample(
			"success", CostSummaryListJsonVO::createShared());
	)
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/chargedetails/query-summary",
		summaryByEncounter, CostSummaryQuery,
		execSummaryByEncounter(query, authObject->getPayload()));

private:
	CostDetailPageJsonVO::Wrapper execQueryAll(
		const CostDetailQuery::Wrapper& query, const PayloadDTO& payload);
	CostSummaryListJsonVO::Wrapper execSummaryByEncounter(
		const CostSummaryQuery::Wrapper& query, const PayloadDTO& payload);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // !_COSTDETAILCONTROLLER_H_
