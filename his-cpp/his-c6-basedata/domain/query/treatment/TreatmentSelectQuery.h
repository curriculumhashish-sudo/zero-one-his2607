#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: axiao
 @Date: 2026/07/30

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
#ifndef _TREATMENT_SELECT_QUERY_H_
#define _TREATMENT_SELECT_QUERY_H_

#include "../../GlobalInclude.h"
#include "domain/query/PageQuery.h"

#include <cstdint>
#include <limits>

#include OATPP_CODEGEN_BEGIN(DTO)

/** 获取选择诊疗列表的条件分页查询模型。 */
class TreatmentSelectQuery : public PageQuery
{
	DTO_INIT(TreatmentSelectQuery, PageQuery);

	// 诊疗名称、编码、拼音码或五笔码关键字。
	API_DTO_FIELD_REQUIRE(
		String,
		keywords,
		ZH_WORDS_GETTER("treatment.field.keywords"),
		false);
	// 状态：1-草稿，2-启用，3-停用，4-废弃。
	API_DTO_FIELD_REQUIRE(
		Int32,
		status,
		ZH_WORDS_GETTER("treatment.field.status"),
		false);

public:
	/** 校验分页范围和状态范围。 */
	std::string validate() const
	{
		if (!pageIndex || !pageSize)
		{
			return "pagination required.";
		}

		const uint64_t pageIndexValue = pageIndex.getValue(1);
		const uint64_t pageSizeValue = pageSize.getValue(10);
		if (pageIndexValue < 1 || pageIndexValue > MAX_PAGE_INDEX ||
			pageSizeValue < 1 || pageSizeValue > MAX_PAGE_SIZE)
		{
			return "pagination invalid.";
		}

		if (status)
		{
			const int statusValue = status.getValue(0);
			if (statusValue < MIN_STATUS || statusValue > MAX_STATUS)
			{
				return "status invalid.";
			}
		}
		return "";
	}

private:
	static constexpr int MIN_STATUS = 1;
	static constexpr int MAX_STATUS = 4;
	static constexpr uint64_t MAX_PAGE_INDEX =
		static_cast<uint64_t>((std::numeric_limits<std::int32_t>::max)());
	static constexpr uint64_t MAX_PAGE_SIZE = 1000;
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_TREATMENT_SELECT_QUERY_H_
