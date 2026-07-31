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
#ifndef _diagnoseCONTROLLER_H_
#define _diagnoseCONTROLLER_H_
#include "domain/GlobalInclude.h"

// Query
#include "domain/query/Diagnose/DiagnoseCatalogQuery.h"
#include "domain/query/Diagnose/DiagnosePageQuery.h"
//#include "domain/query/Diagnose/DiagnoseDetailQuery.h"
#include "domain/query/Diagnose/DiagnoseSelectQuery.h"
//DTO
#include "domain/dto/Diagnose/DiagnosePageDTO.h"
#include "domain/dto/Diagnose/DiagnoseDetailDTO.h"
#include "domain/dto/Diagnose/DiagnoseSelectDTO.h"
//Json
#include "domain/vo/BaseJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseDetailJsonVO.h"
#include "domain/vo/Diagnose/DiagnosePageJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseSelectJsonVO.h"
#include "domain/vo/Diagnose/DiagnoseJsonVO.h"



using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

#define API_TAG ZH_WORDS_GETTER("diagnose.tags.t1")


class DiagnoseController : public oatpp::web::server::api::ApiController
{
	API_ACCESS_DECLARE(DiagnoseController);

public:
	
	//1. diagnose / select 
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnose.select.summary"), listDiagnoseSelect, DiagnoseSelectQuery, DiagnoseSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnose/select-page", listDiagnoseSelect, DiagnoseSelectQuery, executeListSelectDiagnose(query));
	
	// 2. diagnose/page
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnose.page.summary"), listDiagnosePage, DiagnosePageQuery, DiagnosePageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnose/page", listDiagnosePage, DiagnosePageQuery, executeListPageDiagnose(query));
   
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

	//逻辑批量删，带鉴权
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.delete.summarylogicAuth"),removediagnose,DiagnoseRemoveJsonVO::Wrapper,API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL,"/diagnose",removediagnose,BODY_DTO(DiagnoseRemoveDTO::Wrapper, dto),execRemovediagnose(dto, authObject->getPayload()));

	//物理批量删除（真实删除）
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.delete.summary1"),RemoveDiagnose,ListJsonVO<String>::Wrapper,API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL,"/diagnose",RemoveDiagnose,BODY_DTO(List<String>, ids),execRemoveDiagnose(ids));
	
	//逻辑批量删（不带鉴权）
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.delete.summarylogic"),removeDiagnose,ListJsonVO<String>::Wrapper,API_TAG);
	API_HANDLER_ENDPOINT_AUTH(API_M_DEL,"/diagnose",removeDiagnose,BODY_DTO(List<String>, ids),execremoveDiagnose(ids));

    // 定义批量启用/停用接口描述
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnose.field.diag.status"), batchChangeDiagnoseStatus, StringJsonVO::Wrapper, API_TAG);
	// 定义批量启用/停用接口处理
	API_HANDLER_ENDPOINT_AUTH(API_M_POST, "/diagnose/batch/status", batchChangeDiagnoseStatus, BODY_DTO(DiagnoseBatchStatusDTO::Wrapper, dto), execBatchChangeDiagnoseStatus(dto, authObject->getPayload()));
private:
	
	 //1.select
	DiagnoseSelectJsonVO::Wrapper executeListSelectDiagnose(const DiagnoseSelectQuery::Wrapper& query);
	//2. page
	DiagnosePageJsonVO::Wrapper executeListPageDiagnose(const DiagnosePageQuery::Wrapper& query);
	 //3. detail
	DiagnoseDetailJsonVO::Wrapper executeGetDiagnoseDetail(const String& id);
	
	StringJsonVO::Wrapper execAddDiagnose(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
	//StringJsonVO::Wrapper execAddDiagcategory(const DiagnoseAddDTO::Wrapper& dto, const PayloadDTO& payload);
	StringJsonVO::Wrapper execModifyDiagnose(const DiagnoseDTO::Wrapper& dto, const PayloadDTO& payload);
	//物理批量删除
	ListJsonVO<String>::Wrapper execRemoveDiagnose(const List<String>& ids);
	StringJsonVO::Wrapper execBatchChangeDiagnoseStatus(const DiagnoseBatchStatusDTO::Wrapper& dto, const PayloadDTO& payload);
	//逻辑删除带鉴权
	DiagnoseRemoveJsonVO::Wrapper execRemovediagnose(const DiagnoseRemoveDTO::Wrapper& dto, const PayloadDTO& payload);
	//逻辑删除不带鉴权
	ListJsonVO<String>::Wrapper execremoveDiagnose(const List<String>& ids);
};

#undef API_TAG

#include OATPP_CODEGEN_END(ApiController)

#endif //  _diagnoseCONTROLLER_H_ No newline at end of file
