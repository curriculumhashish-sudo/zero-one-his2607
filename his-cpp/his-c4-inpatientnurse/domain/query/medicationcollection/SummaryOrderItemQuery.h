#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Codex
 @Date: 2026/07/21 00:00:00

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
#ifndef _SUMMARY_ORDER_ITEM_QUERY_
#define _SUMMARY_ORDER_ITEM_QUERY_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SummaryOrderItemQuery : public PageQuery
{
	DTO_INIT(SummaryOrderItemQuery, PageQuery);

	API_DTO_FIELD(String, encounterIds,
		ZH_WORDS_GETTER("medicationcollection.field.encounterIds"), true,
		"337907884548657152");
	API_DTO_FIELD(String, medicineCategoryTab,
		ZH_WORDS_GETTER("medicationcollection.field.medicineCategoryTab"), true,
		"west");
	API_DTO_FIELD(String, therapyFilter,
		ZH_WORDS_GETTER("medicationcollection.field.therapyFilter"), true,
		"all");
	API_DTO_FIELD(String, queryDeadline,
		ZH_WORDS_GETTER("medicationcollection.field.queryDeadline"), false,
		"2026-07-21 23:59:59");
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUMMARY_ORDER_ITEM_QUERY_
