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

class SummaryVersionItemDTO : public oatpp::DTO
{
	DTO_INIT(SummaryVersionItemDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, summaryNo,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNo"), true);
	API_DTO_FIELD_REQUIRE(UInt64, version,
		ZH_WORDS_GETTER("medicationcollection.field.version"), true);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->summaryNo = "MS202607210001";
		dto->version = static_cast<v_uint64>(0);
		return dto;
	}
};

class SummaryFailureDTO : public oatpp::DTO
{
	DTO_INIT(SummaryFailureDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, summaryNo,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNo"), false);
	API_DTO_FIELD_REQUIRE(String, reason,
		ZH_WORDS_GETTER("medicationcollection.field.failureReason"), false);

public:
	SummaryFailureDTO() = default;
};

class SubmitSummaryRequestDTO : public oatpp::DTO
{
	DTO_INIT(SubmitSummaryRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("medicationcollection.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(List<SummaryVersionItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medicationcollection.field.summaryItems"), true);
	// Legacy field retained for clients migrating to versioned items.
	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), false);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->idempotencyKey = "submit-20260721-0001";
		dto->items = {SummaryVersionItemDTO::createExample()};
		return dto;
	}
};

class SubmitSummaryResponseDTO : public oatpp::DTO
{
	DTO_INIT(SubmitSummaryResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, successSummaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.successSummaryNos"), false);
	API_DTO_FIELD_REQUIRE(List<String>, failedSummaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.failedSummaryNos"), false);
	API_DTO_FIELD_REQUIRE(List<SummaryFailureDTO::Wrapper>, failures,
		ZH_WORDS_GETTER("medicationcollection.field.failures"), false);
	// Legacy response field mirrors successful summary numbers.
	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medicationcollection.field.message"), false);

public:
	SubmitSummaryResponseDTO()
	{
		successSummaryNos = List<String>::createShared();
		failedSummaryNos = List<String>::createShared();
		failures = List<SummaryFailureDTO::Wrapper>::createShared();
		summaryNos = List<String>::createShared();
	}
};

class CancelSummaryRequestDTO : public oatpp::DTO
{
	DTO_INIT(CancelSummaryRequestDTO, DTO);

	API_DTO_FIELD_REQUIRE(String, idempotencyKey,
		ZH_WORDS_GETTER("medicationcollection.field.idempotencyKey"), true);
	API_DTO_FIELD_REQUIRE(List<SummaryVersionItemDTO::Wrapper>, items,
		ZH_WORDS_GETTER("medicationcollection.field.summaryItems"), true);
	// Legacy field retained for clients migrating to versioned items.
	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), false);

public:
	static Wrapper createExample()
	{
		auto dto = createShared();
		dto->idempotencyKey = "cancel-20260721-0001";
		dto->items = {SummaryVersionItemDTO::createExample()};
		return dto;
	}
};

class CancelSummaryResponseDTO : public oatpp::DTO
{
	DTO_INIT(CancelSummaryResponseDTO, DTO);

	API_DTO_FIELD_REQUIRE(List<String>, successSummaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.successSummaryNos"), false);
	API_DTO_FIELD_REQUIRE(List<String>, failedSummaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.failedSummaryNos"), false);
	API_DTO_FIELD_REQUIRE(List<SummaryFailureDTO::Wrapper>, failures,
		ZH_WORDS_GETTER("medicationcollection.field.failures"), false);
	// Legacy response field mirrors successful summary numbers.
	API_DTO_FIELD_REQUIRE(List<String>, summaryNos,
		ZH_WORDS_GETTER("medicationcollection.field.summaryNos"), false);
	API_DTO_FIELD_REQUIRE(String, message,
		ZH_WORDS_GETTER("medicationcollection.field.message"), false);

public:
	CancelSummaryResponseDTO()
	{
		successSummaryNos = List<String>::createShared();
		failedSummaryNos = List<String>::createShared();
		failures = List<SummaryFailureDTO::Wrapper>::createShared();
		summaryNos = List<String>::createShared();
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_SUMMARY_SLIP_DTO_
