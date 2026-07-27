#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/20 11:00:34

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
#ifndef _DIAGNOSIS_CONTROLLER_H_
#define _DIAGNOSIS_CONTROLLER_H_

#include "domain/query/Diagnosis/DiagnosisPageQuery.h"
#include "domain/query/Diagnosis/DiagnosisDetailQuery.h"
#include "domain/query/Diagnosis/DiagnosisSelectQuery.h"
#include "domain/dto/Diagnosis/DiagnoseDTO.h"
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisDetailJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisDetailPageJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisSelectPageJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisJsonVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnose.tags.t1")
#define API_TAG2 ZH_WORDS_GETTER("diagnose.tags.t2")

/**
 * HTTP endpoints for diagnosis catalog management.
 */
class DiagnosisController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnosisController);

public:
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("diagnose.query_select.summary"),
		querySelectDiagnosis,
		DiagnosisSelectPageJsonVO::Wrapper,
		API_TAG,
		API_DEF_QUERY_PARAM_BUILD(DiagnosisSelectQuery);
		info->queryParams["keyword"].addExample("default", oatpp::String("GM"));
		info->queryParams["diagnosisKindCode"].addExample("default", oatpp::String("1")););
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/diagnosis/query-select",
		querySelectDiagnosis,
		DiagnosisSelectQuery,
		execQuerySelectDiagnosis(query));

	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("diagnose.query_all.summary"),
		queryAllDiagnosis,
		DiagnosisPageQuery,
		DiagnosisDetailPageJsonVO::Wrapper,
		API_TAG2);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/diagnosis/query-all",
		queryAllDiagnosis,
		DiagnosisPageQuery,
		execQueryAllDiagnosis(query, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_QUERY_AUTH(
		ZH_WORDS_GETTER("diagnose.query_one.summary"),
		queryOneDiagnosis,
		DiagnosisDetailQuery,
		DiagnosisDetailJsonVO::Wrapper,
		API_TAG2);
	API_HANDLER_ENDPOINT_QUERY_AUTH(
		API_M_GET,
		"/diagnosis/query-one",
		queryOneDiagnosis,
		DiagnosisDetailQuery,
		execQueryOneDiagnosis(query, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("diagnose.post.summary"),
		addDiagnose,
		StringJsonVO::Wrapper,
		API_TAG);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/diagnose",
		addDiagnose,
		BODY_DTO(DiagnoseAddDTO::Wrapper, dto),
		execAddDiagnose(dto, authObject->getPayload()));

	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("diagnose.put.summary"),
		modifyDiagnose,
		StringJsonVO::Wrapper,
		API_TAG);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_PUT,
		"/diagnose",
		modifyDiagnose,
		BODY_DTO(DiagnoseDTO::Wrapper, dto),
		execModifyDiagnose(dto, authObject->getPayload()));

	//物理批量删除
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("diagnose.delete.summary"),
		removeDiagnose,
		ListJsonVO<String>::Wrapper,
		API_TAG);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_DEL,
		"/diagnose",
		removeDiagnose,
		BODY_DTO(List<String>, ids),
		execRemoveDiagnose(ids));

	//逻辑批量删除
	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("diagnose.delete.summarylogic"),
		removeDiagnosis,
		DiagnoseRemoveJsonVO::Wrapper,
		API_TAG);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_DEL,
		"/diagnosis",
		removeDiagnosis,
		BODY_DTO(DiagnoseRemoveDTO::Wrapper, dto),
		execRemoveDiagnosis(dto, authObject->getPayload()));



	API_DEF_ENDPOINT_INFO_AUTH(
		ZH_WORDS_GETTER("diagnose.batch_status.summary"),
		batchChangeDiagnoseStatus,
		StringJsonVO::Wrapper,
		API_TAG);
	API_HANDLER_ENDPOINT_AUTH(
		API_M_POST,
		"/diagnose/batch/status",
		batchChangeDiagnoseStatus,
		BODY_DTO(DiagnoseBatchStatusDTO::Wrapper, dto),
		execBatchChangeDiagnoseStatus(dto, authObject->getPayload()));

private:
	DiagnosisSelectPageJsonVO::Wrapper execQuerySelectDiagnosis(const DiagnosisSelectQuery::Wrapper& query);
	DiagnosisDetailPageJsonVO::Wrapper execQueryAllDiagnosis(
		const DiagnosisPageQuery::Wrapper& query,
		const PayloadDTO& payload);
	DiagnosisDetailJsonVO::Wrapper execQueryOneDiagnosis(
		const DiagnosisDetailQuery::Wrapper& query,
		const PayloadDTO& payload);
	StringJsonVO::Wrapper execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execModifyDiagnose(const DiagnoseDTO::Wrapper& dto, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemoveDiagnose(const List<String>& ids);
	StringJsonVO::Wrapper execBatchChangeDiagnoseStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload);
	//逻辑删除
	DiagnoseRemoveJsonVO::Wrapper execRemoveDiagnosis(const DiagnoseRemoveDTO::Wrapper&dto,const PayloadDTO&payload);

};

#undef API_TAG
#undef API_TAG2

#include OATPP_CODEGEN_END(ApiController)

#endif // !_DIAGNOSIS_CONTROLLER_H_
