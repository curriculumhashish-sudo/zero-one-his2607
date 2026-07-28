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

/** 选择诊疗分页查询条件。 */
class DiagnosisSelectQuery : public PageQuery
{
	DTO_INIT(DiagnosisSelectQuery, PageQuery);

	API_DTO_FIELD_REQUIRE(String, keyword, ZH_WORDS_GETTER("diagnose.query.field.keyword"), false);
	API_DTO_FIELD_REQUIRE(String, diagnosisKindCode, ZH_WORDS_GETTER("diagnose.query.field.diagnosis_kind_code"), false);

public:
	static constexpr const char* DIAGNOSIS_KIND_WESTERN = "1";
	static constexpr const char* DIAGNOSIS_KIND_TRADITIONAL_CHINESE = "2";

	/** 校验分页和诊断类型。 */
	std::string validate() const
	{
		const uint64_t pageIndexValue = pageIndex.getValue(1);
		const uint64_t pageSizeValue = pageSize.getValue(10);
		if (pageIndexValue < 1 || pageIndexValue > MAX_PAGE_INDEX ||
			pageSizeValue < 1 || pageSizeValue > MAX_PAGE_SIZE)
		{
			return "pagination invalid.";
		}

		const std::string diagnosisKind = diagnosisKindCode.getValue("");
		if (!diagnosisKind.empty() &&
			diagnosisKind != DIAGNOSIS_KIND_WESTERN &&
			diagnosisKind != DIAGNOSIS_KIND_TRADITIONAL_CHINESE)
		{
			return "diagnosis kind invalid.";
		}

		if (!hasValidUtf8Length(keyword, MAX_KEYWORD_LENGTH))
		{
			return "keyword invalid.";
		}
		return "";
	}

private:
	static constexpr uint64_t MAX_PAGE_SIZE = 100;
	static constexpr uint64_t MAX_PAGE_INDEX = 1000000;
	static constexpr size_t MAX_KEYWORD_LENGTH = 255;

	// 校验 UTF-8 字符数。
	static bool hasValidUtf8Length(const oatpp::String& value, size_t maximumLength)
	{
		if (!value)
		{
			return true;
		}

		const std::string text = value.getValue("");
		const auto* data = reinterpret_cast<const unsigned char*>(text.data());
		const size_t byteCount = text.size();
		size_t characterCount = 0;

		for (size_t index = 0; index < byteCount;)
		{
			const unsigned char lead = data[index];
			size_t sequenceLength = 0;
			if (lead <= 0x7F)
			{
				sequenceLength = 1;
			}
			else if ((lead & 0xE0) == 0xC0)
			{
				sequenceLength = 2;
			}
			else if ((lead & 0xF0) == 0xE0)
			{
				sequenceLength = 3;
			}
			else if ((lead & 0xF8) == 0xF0)
			{
				sequenceLength = 4;
			}
			else
			{
				return false;
			}

			if (index + sequenceLength > byteCount)
			{
				return false;
			}
			for (size_t offset = 1; offset < sequenceLength; ++offset)
			{
				if ((data[index + offset] & 0xC0) != 0x80)
				{
					return false;
				}
			}

			index += sequenceLength;
			if (++characterCount > maximumLength)
			{
				return false;
			}
		}
		return true;
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_DIAGNOSIS_SELECT_QUERY_H_
