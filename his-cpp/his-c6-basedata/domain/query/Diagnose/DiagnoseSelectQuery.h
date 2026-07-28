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
#ifndef _DIAGNOSE_SELECT_QUERY_H_
#define _DIAGNOSE_SELECT_QUERY_H_

#include "../../GlobalInclude.h"
#include "../../DiagnoseConstants.h"
#include "domain/query/PageQuery.h"
#include "StringUtil.h"

#include <cstdint>
#include <limits>

#include OATPP_CODEGEN_BEGIN(DTO)

/** 获取选择诊疗列表的条件分页查询模型。 */
class DiagnoseSelectQuery : public PageQuery
{
	DTO_INIT(DiagnoseSelectQuery, PageQuery);

	// 诊疗名称、编码、拼音码或五笔码关键字。
	API_DTO_FIELD_REQUIRE(String, keyword,
		ZH_WORDS_GETTER("diagnose.query.field.keyword"), false);
	// 诊疗类型：1-西医，2-中医。
	API_DTO_FIELD_REQUIRE(String, diagnosisKindCode,
		ZH_WORDS_GETTER("diagnose.query.field.diagnosis_kind_code"), false);

public:
	/** 校验分页范围和诊疗类型。 */
	std::string validate() const
	{
		// 分页参数必须显式传入。
		if (!pageIndex || !pageSize)
		{
			return "pagination required.";
		}

		const uint64_t pageIndexValue = pageIndex.getValue(1);
		const uint64_t pageSizeValue = pageSize.getValue(10);
		// 限制分页范围，避免偏移量溢出或一次读取过多数据。
		if (pageIndexValue < 1 || pageIndexValue > MAX_PAGE_INDEX ||
			pageSizeValue < 1 || pageSizeValue > MAX_PAGE_SIZE)
		{
			return "pagination invalid.";
		}

		std::string diagnosisKind = diagnosisKindCode.getValue("");
		StringUtil::trim(&diagnosisKind);
		// 空值表示不限制诊疗类型。
		if (!diagnosisKind.empty() &&
			diagnosisKind != DiagnoseConstants::KIND_FILTER_WESTERN &&
			diagnosisKind != DiagnoseConstants::KIND_FILTER_TCM)
		{
			return "diagnosis kind invalid.";
		}

		return "";
	}

private:
	static constexpr uint64_t MAX_PAGE_INDEX =
		static_cast<uint64_t>((std::numeric_limits<std::int32_t>::max)());
	static constexpr uint64_t MAX_PAGE_SIZE = 1000;
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSE_SELECT_QUERY_H_
