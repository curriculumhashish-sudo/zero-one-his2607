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

// 引入两套查询Query
#include "domain/query/Diagnosis/DiagnosisSelectQuery.h"
#include "domain/query/Diagnosis/DiagnosisPageQuery.h"
// 引入三套分页/详情返回VO
#include "domain/vo/diagnosis/DiagnosisSelectJsonVO.h"
#include "domain/vo/diagnosis/DiagnosisPageJsonVO.h"
#include "domain/vo/diagnosis/DiagnosisDetailJsonVO.h"

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

#define API_TAG ZH_WORDS_GETTER("diagnosis.tag")

class DiagnosisController : public oatpp::web::server::api::ApiController
{
	// 权限访问声明
	API_ACCESS_DECLARE(DiagnosisController);
public:
	/**
	 * 1. 弹窗选择诊断分页接口 /diagnosis/select-page
	 * 用于医生站、住院登记弹窗筛选（仅展示启用数据，带西医/中医分类筛选）
	 */
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.select.summary"), listSelectDiagnosis, DiagnosisSelectJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/select-page", listSelectDiagnosis, DiagnosisSelectQuery, executeListSelectDiagnosis(query));

	/**
	 * 2. 诊断管理后台分页接口 /diagnosis/page
	 * 后台主列表，支持目录、编码、名称、状态筛选，支持批量启停
	 */
	API_DEF_ENDPOINT_INFO_QUERY_AUTH(ZH_WORDS_GETTER("diagnosis.page.summary"), listDiagnosis, DiagnosisPageJsonVO::Wrapper, API_TAG);
	API_HANDLER_ENDPOINT_QUERY_AUTH(API_M_GET, "/diagnosis/page", listDiagnosis, DiagnosisPageQuery, executeListDiagnosis(query));

	/**
	 * 3. 获取诊断单条详情接口 /diagnosis/detail
	 * 用于详情弹窗、编辑表单回填
	 */
	API_DEF_ENDPOINT_INFO_AUTH(ZH_WORDS_GETTER("diagnosis.detail.summary"), getDiagnosisDetail, DiagnosisDetailJsonVO::Wrapper, API_TAG,
		API_DEF_ADD_QUERY_PARAMS(String, "id", ZH_WORDS_GETTER("diagnosis.field.id"), "", true);
	);
	API_HANDLER_ENDPOINT_AUTH(API_M_GET, "/diagnosis/detail", getDiagnosisDetail, QUERY(String, id), executeGetDiagnosisDetail(id));

private:
	// 弹窗选择分页业务实现
	DiagnosisSelectJsonVO::Wrapper executeListSelectDiagnosis(const DiagnosisSelectQuery::Wrapper& query);
	// 管理后台分页业务实现
	DiagnosisPageJsonVO::Wrapper executeListDiagnosis(const DiagnosisPageQuery::Wrapper& query);
	// 根据ID查询单条详情实现
	DiagnosisDetailJsonVO::Wrapper executeGetDiagnosisDetail(const String& id);
};

#undef API_TAG
#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif // _DIAGNOSISCONTROLLER_H_
