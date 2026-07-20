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
#ifndef _DIAGNOSIS_SELECT_QUERY_
#define _DIAGNOSIS_SELECT_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 诊断弹窗选择查询条件
 * 对应页面：医生站/住院登记诊断选择弹窗
 */
class DiagnosisSelectQuery : public PageQuery
{
	DTO_INIT(DiagnosisSelectQuery, PageQuery);

	// 诊断编码检索
	API_DTO_FIELD_DEFAULT(String, diagnosisCode, ZH_WORDS_GETTER("diagnosis.field.code"));
	// 诊断名称检索
	API_DTO_FIELD_DEFAULT(String, diagnosisName, ZH_WORDS_GETTER("diagnosis.field.name"));
	// 诊断分类 1-西医 2-中医 0-全部
	API_DTO_FIELD_DEFAULT(Int32, diagnosisType, ZH_WORDS_GETTER("diagnosis.field.type"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSIS_SELECT_QUERY_