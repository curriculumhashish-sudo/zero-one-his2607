#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/24 00:00:00

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
#ifndef _SUMMARY_SLIP_DTO_
#define _SUMMARY_SLIP_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class SubmitSummaryRequestDTO : public oatpp::DTO
{
	DTO_INIT(SubmitSummaryRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), true);

public:
	SubmitSummaryRequestDTO()
	{
		summaryNos = List<String>::createShared();
		summaryNos->push_back("MS202607210001");
	}
};

class SubmitSummaryResponseDTO : public oatpp::DTO
{
	DTO_INIT(SubmitSummaryResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medicationcollection.field.message"), false);

public:
	SubmitSummaryResponseDTO()
	{
		summaryNos = List<String>::createShared();
		summaryNos->push_back("MS202607210001");
		message = "Summary submitted to pharmacy.";
	}
};

class CancelSummaryRequestDTO : public oatpp::DTO
{
	DTO_INIT(CancelSummaryRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), true);

public:
	CancelSummaryRequestDTO()
	{
		summaryNos = List<String>::createShared();
		summaryNos->push_back("MS202607210001");
	}
};

class CancelSummaryResponseDTO : public oatpp::DTO
{
	DTO_INIT(CancelSummaryResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medicationcollection.field.message"), false);

public:
	CancelSummaryResponseDTO()
	{
		summaryNos = List<String>::createShared();
		summaryNos->push_back("MS202607210001");
		message = "Summary cancelled.";
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUMMARY_SLIP_DTO_
