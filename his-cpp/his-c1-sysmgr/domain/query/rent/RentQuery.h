#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2026/07/18 19:10:05

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
#ifndef _RENTQUERY_H_
#define _RENTQUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class RentQuery : public PageQuery
{
	DTO_INIT(RentQuery, PageQuery);
	// ×â»§±àÂë
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("rent.field.code"),false);
	// ¹Ø¼ü´Ê
	API_DTO_FIELD_REQUIRE(String, keywords, ZH_WORDS_GETTER("rent.field.keywords"), false);
	// ×â»§×´Ì¬
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("rent.field.status"), false);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_RENTQUERY_H_