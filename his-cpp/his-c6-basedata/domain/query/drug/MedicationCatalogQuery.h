#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: bandao
 @Date: 2026/07/20

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
#ifndef _MEDICATION_CATALOG_QUERY_H_
#define _MEDICATION_CATALOG_QUERY_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class MedicationCatalogPageQuery : public oatpp::DTO
{
	DTO_INIT(MedicationCatalogPageQuery, DTO);

	API_DTO_FIELD(Int32, pageNo, ZH_WORDS_GETTER("medication.field.pageNo"), true, 1);
	API_DTO_FIELD(Int32, pageSize, ZH_WORDS_GETTER("medication.field.pageSize"), true, 50);
	API_DTO_FIELD(String, keyword, ZH_WORDS_GETTER("medication.field.keyword"), false, u8"\u5934\u5B62");
	API_DTO_FIELD(String, name, ZH_WORDS_GETTER("medication.field.name"), false, u8"\u5934\u5B62\u514B\u809F\u80F6\u56CA");
	API_DTO_FIELD(String, categoryCode, ZH_WORDS_GETTER("medication.field.categoryCode"), false, "2");
	DTO_FIELD(List<String>, where) = List<String>::createShared();
	DTO_FIELD_INFO(where)
	{
		info->description = ZH_WORDS_GETTER("medication.field.where");
		info->required = false;
	}

public:
	MedicationCatalogPageQuery()
	{
		where->push_back("name");
		where->push_back("merchandiseName");
		where->push_back("pyStr");
		where->push_back("wbStr");
		where->push_back("merchandisePyStr");
		where->push_back("merchandiseWbStr");
		where->push_back("busNo");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_MEDICATION_CATALOG_QUERY_H_
