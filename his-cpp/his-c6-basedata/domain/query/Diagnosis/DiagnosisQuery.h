#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2022/10/25 11:36:29

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
#ifndef _SAMPLE_QUERY_
#define _SAMPLE_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 定义一个查询诊断的信息传输模型
 */
class DiagnosisQuery : public PageQuery
{
	//定义初始化
	DTO_INIT(DiagnosisQuery, PageQuery);

	// 诊断编码 
	API_DTO_FIELD_DEFAULT(String, diagnosisCode, ZH_WORDS_GETTER("diagnosis.flield.Code"));
	// 诊断名称 
	API_DTO_FIELD_DEFAULT(String, diagnosisName, ZH_WORDS_GETTER("diagnosis.flield.Name"));
	////序号
	//API_DTO_FIELD_DEFAULT(Int32, diagnosisId, ZH_WORDS_GETTER"diagnosis.flield.Id");
	// 诊断分类 
	API_DTO_FIELD_DEFAULT(String, diagnosisType, ZH_WORDS_GETTER("diagnosis.flield.Type"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_QUERY_#pragma once