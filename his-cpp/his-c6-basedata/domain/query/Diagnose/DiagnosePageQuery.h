#pragma once
/*
 Copyright Zero One Star. All rights reserved.
 @Author: awei
 @Date: 2026/07/28
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
#ifndef _DIAGNOSE_PAGE_QUERY_H_
#define _DIAGNOSE_PAGE_QUERY_H_
#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"
#include OATPP_CODEGEN_BEGIN(DTO)

//right 
class DiagnosePageQuery : public PageQuery
{
	DTO_INIT(DiagnosePageQuery, PageQuery);
	// keyword
	API_DTO_FIELD_DEFAULT(String, keyword, ZH_WORDS_GETTER("diagnose.field.keyword"));
	// from  catalog
	API_DTO_FIELD_DEFAULT(Int32, diagnoseType, ZH_WORDS_GETTER("diagnose.field.type"));
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_DIAGNOSE_SELECT_QUERY_H_