#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/21 00:00:00

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
#ifndef _MEDICATION_COLLECTION_CONTROLLER_
#define _MEDICATION_COLLECTION_CONTROLLER_

#include "domain/dto/medicationcollection/MedicationCollectionDTO.h"
#include "domain/query/medicationcollection/SummaryOrderItemQuery.h"
#include "domain/vo/medicationcollection/MedicationCollectionVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("medicationcollection.tag")

class MedicationCollectionController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(MedicationCollectionController);

public:
	// Query medication-order items eligible for summary collection.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicationcollection.query-summary-order-items.summary"),
		querySummaryOrderItems,
		SummaryOrderItemPageJsonVO::Wrapper,
		API_TAG,
		API_DEF_QUERY_PARAM_BUILD(SummaryOrderItemQuery);
		info->responses[Status::CODE_200].addExample(
			"success", SummaryOrderItemPageJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/medicationcollection/query-summary-order-items",
		querySummaryOrderItems,
		SummaryOrderItemQuery,
		execQuerySummaryOrderItems(query));

	// Create medication summary slips from the selected dispense items.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicationcollection.summary-medication.summary"),
		summaryMedication,
		SummaryMedicationJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", SummaryMedicationDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", SummaryMedicationJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/medicationcollection/summary-medication",
		summaryMedication,
		BODY_DTO(SummaryMedicationDTO::Wrapper, dto),
		execSummaryMedication(dto));

private:
	SummaryOrderItemPageJsonVO::Wrapper execQuerySummaryOrderItems(
		const SummaryOrderItemQuery::Wrapper& query);
	SummaryMedicationJsonVO::Wrapper execSummaryMedication(
		const SummaryMedicationDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICATION_COLLECTION_CONTROLLER_
