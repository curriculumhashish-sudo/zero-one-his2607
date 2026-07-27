#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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

#include "domain/dto/medicationcollection/CreateMedicationSummaryDTO.h"
#include "domain/dto/medicationcollection/SummarySlipDTO.h"
#include "domain/query/medicationcollection/PagePendingDispenseSummaryLinesQuery.h"
#include "domain/vo/medicationcollection/CreateMedicationSummaryVO.h"
#include "domain/vo/medicationcollection/PagePendingDispenseSummaryLinesVO.h"
#include "domain/vo/medicationcollection/SummarySlipVO.h"

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
		PagePendingDispenseSummaryLinesJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_PAGE_PARAMS();
		info->queryParams["pageIndex"].required = true;
		info->queryParams["pageSize"].required = true;
		API_DEF_ADD_QUERY_PARAMS(String, "encounterIds",
			ZH_WORDS_GETTER("medicationcollection.field.encounterIds"),
			"337907884548657152,337907884548657153", true);
		API_DEF_ADD_QUERY_PARAMS(String, "medicineCategoryTab",
			ZH_WORDS_GETTER("medicationcollection.field.medicineCategoryTab"), "west", true);
		API_DEF_ADD_QUERY_PARAMS(String, "therapyFilter",
			ZH_WORDS_GETTER("medicationcollection.field.therapyFilter"), "all", true);
		API_DEF_ADD_QUERY_PARAMS(String, "queryDeadline",
			ZH_WORDS_GETTER("medicationcollection.field.queryDeadline"), "2026-07-21 23:59:59", false);
		API_DEF_ADD_QUERY_PARAMS(String, "keyword",
			ZH_WORDS_GETTER("medicationcollection.field.keyword"), "替米沙坦", false);
		API_DEF_ADD_QUERY_PARAMS(String, "startDate",
			ZH_WORDS_GETTER("medicationcollection.field.startDate"), "2026-07-21 00:00:00", false);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate",
			ZH_WORDS_GETTER("medicationcollection.field.endDate"), "2026-07-21 23:59:59", false);
		API_DEF_ADD_QUERY_PARAMS(String, "sortBy",
			ZH_WORDS_GETTER("medicationcollection.field.sortBy"), "plannedDispenseTime", false);
		API_DEF_ADD_QUERY_PARAMS(String, "sortDirection",
			ZH_WORDS_GETTER("medicationcollection.field.sortDirection"), "DESC", false);
		API_DEF_ADD_QUERY_PARAMS(String, "where",
			ZH_WORDS_GETTER("medicationcollection.field.where"), "medicationName", false);
		info->responses[Status::CODE_200].addExample(
			"success", PagePendingDispenseSummaryLinesJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/medicationcollection/query-summary-order-items",
		querySummaryOrderItems,
		PagePendingDispenseSummaryLinesQuery,
		execQuerySummaryOrderItems(query));

	// Create medication summary slips from the selected dispense items.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicationcollection.summary-medication.summary"),
		summaryMedication,
		CreateMedicationSummaryJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", CreateMedicationSummaryDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", CreateMedicationSummaryJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/medicationcollection/summary-medication",
		summaryMedication,
		BODY_DTO(CreateMedicationSummaryDTO::Wrapper, dto),
		execSummaryMedication(dto, authObject->getPayload()));

	// Submit summary slips to pharmacy (batch)
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicationcollection.submit-summary.summary"),
		submitSummary,
		SubmitSummaryJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", SubmitSummaryRequestDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", SubmitSummaryJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/medicationcollection/submit-summary",
		submitSummary,
		BODY_DTO(SubmitSummaryRequestDTO::Wrapper, dto),
		execSubmitSummary(dto));

	// Cancel summary slips (batch revoke)
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("medicationcollection.cancel-summary.summary"),
		cancelSummary,
		CancelSummaryJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", CancelSummaryRequestDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", CancelSummaryJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/medicationcollection/cancel-summary",
		cancelSummary,
		BODY_DTO(CancelSummaryRequestDTO::Wrapper, dto),
		execCancelSummary(dto));

private:
	PagePendingDispenseSummaryLinesJsonVO::Wrapper execQuerySummaryOrderItems(
		const PagePendingDispenseSummaryLinesQuery::Wrapper& query);
	CreateMedicationSummaryJsonVO::Wrapper execSummaryMedication(
		const CreateMedicationSummaryDTO::Wrapper& dto,const PayloadDTO& payload);
	SubmitSummaryJsonVO::Wrapper execSubmitSummary(
		const SubmitSummaryRequestDTO::Wrapper& dto);
	CancelSummaryJsonVO::Wrapper execCancelSummary(
		const CancelSummaryRequestDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_MEDICATION_COLLECTION_CONTROLLER_
