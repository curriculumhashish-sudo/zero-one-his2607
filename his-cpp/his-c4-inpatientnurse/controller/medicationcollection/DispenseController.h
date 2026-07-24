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
#ifndef _DISPENSE_CONTROLLER_
#define _DISPENSE_CONTROLLER_

#include "domain/vo/BaseJsonVO.h"
#include "domain/query/medicationcollection/DispenseSummaryQuery.h"
#include "domain/dto/medicationcollection/DispenseDTO.h"
#include "domain/vo/medicationcollection/DispenseVO.h"

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("medicationcollection.tag")

/**
 * Summary drug-collection (dispense bill) controller.
 * API 1: query summary bill list by status/date with pagination.
 * API 2: query dispense details by summary bill no.
 */
class DispenseController : public oatpp::web::server::api::ApiController
{
	// declare controller access
	API_ACCESS_DECLARE(DispenseController);
public:
	// Define the summary bill list query.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dispense.summary-list.summary"),
		querySummaryList,
		DispenseSummaryPageJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_PAGE_PARAMS();
		info->queryParams["pageIndex"].required = true;
		info->queryParams["pageSize"].required = true;
		API_DEF_ADD_QUERY_PARAMS(String, "statusEnum", ZH_WORDS_GETTER("dispense.field.status-enum"), "WAIT_REVIEW", false);
		API_DEF_ADD_QUERY_PARAMS(String, "dispenseDate", ZH_WORDS_GETTER("dispense.field.dispense-date"), "2026-07-19", false);
		info->responses[Status::CODE_200].addExample(
			"success", DispenseSummaryPageJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/medicationcollection/summary",
		querySummaryList,
		DispenseSummaryQuery,
		execQuerySummaryList(query));

	// Define the dispense detail query.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("dispense.detail.summary"),
		queryDetail,
		DispenseDetailListJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "summaryNo", ZH_WORDS_GETTER("dispense.field.summary-no"), "SUM20260719001", true);
		info->responses[Status::CODE_200].addExample(
			"success", DispenseDetailListJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/medicationcollection/detail",
		queryDetail,
		DispenseDetailQuery,
		execQueryDetail(query));
private:
	// Execute the summary bill list query.
	DispenseSummaryPageJsonVO::Wrapper execQuerySummaryList(const DispenseSummaryQuery::Wrapper& query);
	// Execute the dispense detail query.
	DispenseDetailListJsonVO::Wrapper execQueryDetail(const DispenseDetailQuery::Wrapper& query);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen
#endif // !_DISPENSE_CONTROLLER_
