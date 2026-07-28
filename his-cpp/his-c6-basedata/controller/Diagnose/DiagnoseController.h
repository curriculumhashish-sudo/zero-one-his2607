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
#include "GlobalInclude.h"


// Query
#include "domain/query/Diagnose/DiagnosePageQuery.h"
#include "domain/query/Diagnose/DiagnoseDetailQuery.h"
#include "domain/query/Diagnose/DiagnoseSelectQuery.h"
//DTO
#include "domain/dto/Diagnose/DiagnosePageDTO.h"
#include "domain/dto/Diagnose/DiagnoseDetailDTO.h"
#include "domain/dto/Diagnose/DiagnoseSelectDTO.h"
//Json
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseDetailJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseDetailPageJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseSelectPageJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseJsonVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnose.tags.t1")


class DiagnoseController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnoseController);

public:
	//1. diagnose / select - page
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnose.select.summary"), listSelectDiagnose, DiagnoseSelectQuery, DiagnoseSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnose/select-page", listSelectDiagnose, DiagnoseSelectQuery, executeListSelectDiagnose(query));
	
	// 2. diagnose/page
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnose.page.summary"), listDiagnose, DiagnosePageQuery, DiagnosePageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnose/page", listDiagnose, DiagnosePageQuery, executeListDiagnose(query));
   
	// 3. diagnose/detail
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.detail.summary"), getDiagnoseDetail, DiagnoseDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("diagnose.field.id"), "ae65c714d48d4f34b52479f5482c0edd", true);
    );
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/diagnose/detail", getDiagnoseDetail, QUERY(String, id), executeGetDiagnoseDetail(id));

    // 定义新增诊断表接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.post.summary"), addDiagnose, StringJsonVO::Wrapper, API_TAG);
	// 定义新增诊断表接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose", addDiagnose, BODY_DTO(DiagnoseAddDTO::Wrapper, dto), execAddDiagnose(dto, authObject->getPayload()));

	//  定义修改接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.put.summary"), modifyDiagnose, StringJsonVO::Wrapper, API_TAG);
	// 定义修改接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_PUT, "/diagnose", modifyDiagnose, BODY_DTO(DiagnoseDTO::Wrapper, dto), execModifyDiagnose(dto, authObject->getPayload()));

	//  定义删除接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.delete.summary"), removeDiagnose, ListJsonVO<String>::Wrapper, API_TAG);
	//  定义删除接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL, "/diagnose", removeDiagnose, BODY_DTO(List<String>, ids), execRemoveDiagnose(ids));

	// 定义批量启用/停用接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.field.diag.status"), batchChangeDiagnoseStatus, StringJsonVO::Wrapper, API_TAG);
	// 定义批量启用/停用接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose/batch/status", batchChangeDiagnoseStatus, BODY_DTO(DiagnoseBatchStatusDTO::Wrapper, dto), execBatchChangeDiagnoseStatus(dto, authObject->getPayload()));
private:
	// select
	DiagnoseSelectJsonVO::Wrapper executeListSelectDiagnose(const DiagnoseSelectQuery::Wrapper& query);
	// 2. page
	DiagnosePageJsonVO::Wrapper executeListDiagnose(const DiagnosePageQuery::Wrapper& query);
	// 3. detail
	DiagnoseDetailJsonVO::Wrapper executeGetDiagnoseDetail(const String& id);
	// 新增诊断表数据
	StringJsonVO::Wrapper execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
	//  新增诊断目录（分类）数据
	//StringJsonVO::Wrapper execAddDiagcategory(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
	// 演示修改数据
	StringJsonVO::Wrapper execModifyDiagnose(const DiagnoseDTO::Wrapper& dto, const PayloadDTO& payload);
	// 演示删除数据
	ListJsonVO<String>::Wrapper execRemoveDiagnose(const List<String>& ids);
	//演示批量启用/停用
	StringJsonVO::Wrapper execBatchChangeDiagnoseStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif //  _DIAGNOSISCONTROLLER_H_ No newline at end of file
