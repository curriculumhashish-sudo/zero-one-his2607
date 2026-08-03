#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/08/03

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
#ifndef _TREATMENT_OPTIONS_QUERY_H_
#define _TREATMENT_OPTIONS_QUERY_H_

#include "../../GlobalInclude.h"
#include <cstdint>
#include <limits>

#include OATPP_CODEGEN_BEGIN(DTO)

/** 新版诊疗选择列表分页查询参数。 */
class TreatmentOptionsQuery : public oatpp::DTO
{
	DTO_INIT(TreatmentOptionsQuery, DTO);

	API_DTO_FIELD_REQUIRE(
		UInt64,
		pageIndex,
		ZH_WORDS_GETTER("treatment.field.page-index"),
		true);
	API_DTO_FIELD_REQUIRE(
		UInt64,
		pageSize,
		ZH_WORDS_GETTER("treatment.field.page-size"),
		true);

	API_DTO_FIELD_REQUIRE(
		String,
		keyword,
		ZH_WORDS_GETTER("treatment.field.keyword"),
		false);
	API_DTO_FIELD_REQUIRE(
		String,
		categoryCode,
		ZH_WORDS_GETTER("treatment.field.category-code"),
		false);

public:
	std::string validate() const
	{
		if (!pageIndex || !pageSize)
		{
			return "pagination required.";
		}

		const uint64_t pageIndexValue = pageIndex.getValue(0);
		const uint64_t pageSizeValue = pageSize.getValue(0);
		if (pageIndexValue < 1 || pageIndexValue > MAX_PAGE_INDEX ||
			pageSizeValue < 1 || pageSizeValue > MAX_PAGE_SIZE)
		{
			return "pagination invalid.";
		}
		return "";
	}

private:
	static constexpr uint64_t MAX_PAGE_INDEX =
		static_cast<uint64_t>((std::numeric_limits<std::int32_t>::max)());
	static constexpr uint64_t MAX_PAGE_SIZE = 1000;
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENT_OPTIONS_QUERY_H_
