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
#ifndef _DIAGNOSIS_SELECT_QUERY_H_
#define _DIAGNOSIS_SELECT_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Query conditions for the paged diagnosis picker.
 */
class DiagnosisSelectQuery : public PageQuery
{
	DTO_INIT(DiagnosisSelectQuery, PageQuery);

	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("diagnose.query.field.keyword"), false);
	API_DTO_FIELD_REQUIRE(String, diagnosisKindCode, ZH_WORDS_GETTER("diagnose.query.field.diagnosis_kind_code"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSIS_SELECT_QUERY_H_
