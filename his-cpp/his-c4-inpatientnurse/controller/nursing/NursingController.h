#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

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
#ifndef _NURSING_CONTROLLER_H_
#define _NURSING_CONTROLLER_H_

#include "domain/query/nursing/NursingRecordQuery.h"
#include "domain/dto/nursing/NursingRecordDTO.h"
#include "domain/vo/nursing/NursingRecordVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("nursing.tags.tag")

class NursingController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(NursingController);

public:
	// List nursing records by encounter and date range
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("nursing.query.summary"),
		listNursingRecords,
		NursingRecordListJsonVO::Wrapper,
		API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "encounterId",
			ZH_WORDS_GETTER("nursing.field.encounterId"), "337907884548657152", true);
		API_DEF_ADD_QUERY_PARAMS(String, "patientId",
			ZH_WORDS_GETTER("nursing.field.patientId"), "326940849401761792", true);
		API_DEF_ADD_QUERY_PARAMS(String, "recordedAtFrom",
			ZH_WORDS_GETTER("nursing.field.recordedAtFrom"), "2026-07-01", false);
		API_DEF_ADD_QUERY_PARAMS(String, "recordedAtTo",
			ZH_WORDS_GETTER("nursing.field.recordedAtTo"), "2026-07-24", false);
		API_DEF_ADD_QUERY_PARAMS(Boolean, "sortAsc",
			ZH_WORDS_GETTER("nursing.field.sortAsc"), false, false);
		info->responses[Status::CODE_200].addExample(
			"success", NursingRecordListJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/nursing/list-records",
		listNursingRecords,
		NursingRecordQuery,
		execListNursingRecords(query));

	// Save nursing records in batch
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("nursing.add.summary"),
		saveNursingRecords,
		NursingRecordSaveJsonVO::Wrapper,
		API_TAG,
		info->body.addExample("default", NursingRecordSaveRequestDTO::createShared());
		info->responses[Status::CODE_200].addExample(
			"success", NursingRecordSaveJsonVO::createShared());
	);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/nursing/save-records",
		saveNursingRecords,
		BODY_DTO(NursingRecordSaveRequestDTO::Wrapper, dto),
		execSaveNursingRecords(dto));

private:
	NursingRecordListJsonVO::Wrapper execListNursingRecords(
		const NursingRecordQuery::Wrapper& query);
	NursingRecordSaveJsonVO::Wrapper execSaveNursingRecords(
		const NursingRecordSaveRequestDTO::Wrapper& dto);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif // !_NURSING_CONTROLLER_H_
