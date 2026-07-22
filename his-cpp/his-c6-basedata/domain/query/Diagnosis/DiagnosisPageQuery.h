#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/20 11:36:29
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
#ifndef _DIAGNOSIS_PAGE_QUERY_
#define _DIAGNOSIS_PAGE_QUERY_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)
/**
 * 诊断管理列表查询条件
 * 对应页面：诊断后台管理主列表
 */
class DiagnosisPageQuery : public PageQuery
{
	DTO_INIT(DiagnosisPageQuery, PageQuery);
	// 诊断编码检索
	API_DTO_FIELD_DEFAULT(String, diagnosisCode, ZH_WORDS_GETTER("diagnosis.field.code"));
	// 诊断名称检索
	API_DTO_FIELD_DEFAULT(String, diagnosisName, ZH_WORDS_GETTER("diagnosis.field.name"));
	// 诊断目录筛选
	API_DTO_FIELD_DEFAULT(String, catalogName, ZH_WORDS_GETTER("diagnosis.field.catalogName"));
	// 启用状态 0-停用 1-启用 0-全部
	API_DTO_FIELD_DEFAULT(Int32, status, ZH_WORDS_GETTER("diagnosis.field.status"));
};
#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSIS_PAGE_QUERY_