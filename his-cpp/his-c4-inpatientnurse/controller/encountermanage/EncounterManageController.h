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
#ifndef _ENCOUNTER_MANAGE_CONTROLLER_
#define _ENCOUNTER_MANAGE_CONTROLLER_

#include "domain/dto/encountermanage/PagePatientQueueDTO.h"
#include "domain/dto/encountermanage/PatientStatisticsDTO.h"
#include "domain/query/encountermanage/PagePatientQueueQuery.h"
#include "domain/query/encountermanage/PatientStatisticsQuery.h"
#include "domain/vo/encountermanage/PagePatientQueueVO.h"
#include "domain/vo/encountermanage/PatientStatisticsVO.h"
#include "domain/vo/encountermanage/WardTreeVO.h"
#include "controller/RequestValidation.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("encountermanage.tag")

class EncounterManageController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(EncounterManageController);

public:
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("encountermanage.query-ward-tree.summary"),
		queryWardTree,
		WardTreeJsonVO::Wrapper,
		API_TAG,
		info->responses[Status::CODE_200].addExample("success", WardTreeJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_NOPARAM_AUTH(API_M_GET, "/encountermanage/query-ward-tree", queryWardTree,
		execQueryWardTree(authObject->getPayload()));

	// Query aggregate counts displayed above the patient queue.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("encountermanage.query-patient-statistics.summary"),
		queryPatientStatistics,
		PatientStatisticsJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "keyword",
			ZH_WORDS_GETTER("encountermanage.field.keyword"), ZH_WORDS_GETTER("business-text.sample.patient-name"), false);
		API_DEF_ADD_QUERY_PARAMS(String, "wardId",
			ZH_WORDS_GETTER("encountermanage.field.wardId"), "334526584536674304", false);
		API_DEF_ADD_QUERY_PARAMS(String, "wardIds",
			ZH_WORDS_GETTER("encountermanage.field.wardIds"), "334526584536674304,334526584536674305", false);
		API_DEF_ADD_QUERY_PARAMS(String, "houseId",
			ZH_WORDS_GETTER("encountermanage.field.houseId"), "334526625720438784", false);
		API_DEF_ADD_QUERY_PARAMS(String, "houseIds",
			ZH_WORDS_GETTER("encountermanage.field.houseIds"), "334526625720438784,334526640208363520", false);
		API_DEF_ADD_QUERY_PARAMS(String, "nurseStationExecutionDate",
			ZH_WORDS_GETTER("encountermanage.field.nurseStationExecutionDate"), "2026-07-21", false);
		API_DEF_ADD_QUERY_PARAMS(String, "nurseStationTaskFilter",
			ZH_WORDS_GETTER("encountermanage.field.nurseStationTaskFilter"), "pending-audit", false);
		API_DEF_ADD_QUERY_PARAMS(String, "nurseStationStatusFilter",
			ZH_WORDS_GETTER("encountermanage.field.nurseStationStatusFilter"), "SETTLED_DISCHARGED", false);
		API_DEF_ADD_QUERY_PARAMS(String, "startDate",
			ZH_WORDS_GETTER("encountermanage.field.startDate"), "2026-07-21 00:00:00", false);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate",
			ZH_WORDS_GETTER("encountermanage.field.endDate"), "2026-07-21 23:59:59", false);
		info->responses[Status::CODE_200].addExample(
			"success", PatientStatisticsJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/encountermanage/query-patient-statistics",
		queryPatientStatistics,
		PatientStatisticsQuery,
		execQueryPatientStatistics(query, authObject->getPayload()));

	// Query the inpatient nurse-station patient queue.
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("encountermanage.query-patient-queue.summary"),
		queryPatientQueue,
		PagePatientQueueJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_PAGE_PARAMS();
		info->queryParams["pageIndex"].required = true;
		info->queryParams["pageSize"].required = true;
		API_DEF_ADD_QUERY_PARAMS(String, "keyword",
			ZH_WORDS_GETTER("encountermanage.field.keyword"), ZH_WORDS_GETTER("business-text.sample.patient-name"), false);
		API_DEF_ADD_QUERY_PARAMS(String, "wardId",
			ZH_WORDS_GETTER("encountermanage.field.wardId"), "334526584536674304", false);
		API_DEF_ADD_QUERY_PARAMS(String, "wardIds",
			ZH_WORDS_GETTER("encountermanage.field.wardIds"), "334526584536674304,334526584536674305", false);
		API_DEF_ADD_QUERY_PARAMS(String, "houseId",
			ZH_WORDS_GETTER("encountermanage.field.houseId"), "334526625720438784", false);
		API_DEF_ADD_QUERY_PARAMS(String, "houseIds",
			ZH_WORDS_GETTER("encountermanage.field.houseIds"), "334526625720438784,334526640208363520", false);
		API_DEF_ADD_QUERY_PARAMS(String, "nurseStationExecutionDate",
			ZH_WORDS_GETTER("encountermanage.field.nurseStationExecutionDate"), "2026-07-21", false);
		API_DEF_ADD_QUERY_PARAMS(String, "nurseStationTaskFilter",
			ZH_WORDS_GETTER("encountermanage.field.nurseStationTaskFilter"), "pending-audit", false);
		API_DEF_ADD_QUERY_PARAMS(String, "nurseStationStatusFilter",
			ZH_WORDS_GETTER("encountermanage.field.nurseStationStatusFilter"), "SETTLED_DISCHARGED", false);
		API_DEF_ADD_QUERY_PARAMS(String, "startDate",
			ZH_WORDS_GETTER("encountermanage.field.startDate"), "2026-07-21 00:00:00", false);
		API_DEF_ADD_QUERY_PARAMS(String, "endDate",
			ZH_WORDS_GETTER("encountermanage.field.endDate"), "2026-07-21 23:59:59", false);
		API_DEF_ADD_QUERY_PARAMS(String, "sortBy",
			ZH_WORDS_GETTER("encountermanage.field.sortBy"), "startTime", false);
		API_DEF_ADD_QUERY_PARAMS(String, "sortDirection",
			ZH_WORDS_GETTER("encountermanage.field.sortDirection"), "DESC", false);
		API_DEF_ADD_QUERY_PARAMS(String, "where",
			ZH_WORDS_GETTER("encountermanage.field.where"), "patientName,bedName", false);
		info->responses[Status::CODE_200].addExample(
			"success", PagePatientQueueJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/encountermanage/query-patient-queue",
		queryPatientQueue,
		PagePatientQueueQuery,
		execQueryPatientQueue(query, authObject->getPayload()));

private:
	WardTreeJsonVO::Wrapper execQueryWardTree(const PayloadDTO& payload);
	PatientStatisticsJsonVO::Wrapper execQueryPatientStatistics(
		const PatientStatisticsQuery::Wrapper& query, const PayloadDTO& payload);
	PagePatientQueueJsonVO::Wrapper execQueryPatientQueue(
		const PagePatientQueueQuery::Wrapper& query, const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_ENCOUNTER_MANAGE_CONTROLLER_
