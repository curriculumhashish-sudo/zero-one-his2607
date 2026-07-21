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
#ifndef _DIAGNOSISCONTROLLER_H_
#define _DIAGNOSISCONTROLLER_H_

#include "domain/vo/BaseJsonVO.h"
#include "GlobalInclude.h"

// 查询Query
#include "domain/query/Diagnosis/DiagnosisSelectQuery.h"
#include "domain/query/Diagnosis/DiagnosisPageQuery.h"

// 业务DTO
#include "domain/dto/Diagnosis/DiagnosisAddDTO.h"
#include "domain/dto/Diagnosis/DiagnosisPageDTO.h"
#include "domain/dto/Diagnosis/DiagnosisDetailDTO.h"
#include "domain/dto/Diagnosis/DiagnosisDeleteDTO.h"

// 返回JsonVO
#include "domain/vo/Diagnosis/DiagnosisSelectJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisPageJsonVO.h"
#include "domain/vo/Diagnosis/DiagnosisDetailJsonVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnosis.tag")

class DiagnosisController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnosisController);
public:
	// 1. 诊断弹窗选择分页
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.select.summary"), listSelectDiagnosis, DiagnosisSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/select-page", listSelectDiagnosis, DiagnosisSelectQuery, executeListSelectDiagnosis(query));

	// 2. 诊断管理后台分页
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.page.summary"), listDiagnosis, DiagnosisPageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/page", listDiagnosis, DiagnosisPageQuery, executeListDiagnosis(query));

	// 3. 诊断单条详情
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.detail.summary"), getDiagnosisDetail, DiagnosisDetailJsonVO::Wrapper, API_TAG,
	   API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("diagnosis.field.id"), "", true);
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/diagnosis/detail", getDiagnosisDetail, QUERY(String, id), executeGetDiagnosisDetail(id));

	// 4. 新增诊断
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.post.summary"), addDiagnosis, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnosis", addDiagnosis, BODY_DTO(DiagnosisAddDTO::Wrapper, dto), execAddDiagnosis(dto, authObject->getPayload()));

	// 5. 修改诊断
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.put.summary"), modifyDiagnosis, StringJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnosis", modifyDiagnosis, BODY_DTO(DiagnosisPageDTO::Wrapper, dto), execModifyDiagnosis(dto, authObject->getPayload()));

	// 6. 批量删除诊断
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.delete.summary"), removeDiagnosis, ListJsonVO<String>::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnosis", removeDiagnosis, BODY_DTO(List<String>, ids), execRemoveDiagnosis(ids));

private:
	DiagnosisSelectJsonVO::Wrapper executeListSelectDiagnosis(const DiagnosisSelectQuery::Wrapper& query);
	DiagnosisPageJsonVO::Wrapper executeListDiagnosis(const DiagnosisPageQuery::Wrapper& query);
	DiagnosisDetailJsonVO::Wrapper executeGetDiagnosisDetail(const String& id);
	/*StringJsonVO::Wrapper execAddDiagnosis(const DiagnosisAddDTO::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execModifyDiagnosis(const DiagnosisPageDTO::Wrapper& dto, const PayloadDTO& payload);
	ListJsonVO<String>::Wrapper execRemoveDiagnosis(const List<String>& ids);*/
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController)

#endif // _DIAGNOSISCONTROLLER_H_