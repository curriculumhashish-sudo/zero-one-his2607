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
#ifndef _SAMPLE_FULL_SELECT_QUERY_
#define _SAMPLE_FULL_SELECT_QUERY_

#include "../../GlobalInclude.h"
#include OATPP_CODEGEN_BEGIN(DTO)

class SampleQuery : public oatpp::DTO
{
	DTO_INIT(SampleQuery, DTO);

	DTO_FIELD(UInt64, page_index) = 1;
	DTO_FIELD_INFO(page_index) {
		info->required = true;
#ifndef LINUX
		info->description = u8"当前页码";
#else
		info->description = "current page index";
#endif
	}

	DTO_FIELD(UInt64, page_size) = 10;
	DTO_FIELD_INFO(page_size) {
		info->required = true;
#ifndef LINUX
		info->description = u8"每页数据条数";
#else
		info->description = "page size";
#endif
	}

	// 综合关键词，匹配样本业务编码、样本名称、拼音码、五笔码
	API_DTO_FIELD_DEFAULT(String, keyword, ZH_WORDS_GETTER("sample.query.keyword"));
};

using SampleFullSelectQuery = SampleQuery;

#include OATPP_CODEGEN_END(DTO)
#endif // !_SAMPLE_FULL_SELECT_QUERY_
