#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
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
#ifndef _FREQUENCYDTO_H_
#define _FREQUENCYDTO_H_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)

class FrequencyDTO : public oatpp::DTO
{
	DTO_INIT(FrequencyDTO, DTO);

	// Primary identifier.
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("frequency.field.id"), true);
	// Business code.
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("frequency.field.code"), true);
	// Display name.
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("frequency.field.name"), true);
	// Enable status.
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("frequency.field.status"), true);
	// Cycle type code.
	API_DTO_FIELD_REQUIRE(String, cycleType, ZH_WORDS_GETTER("frequency.field.cycle-type"), true);
	// Comma-separated execution times within one day.
	API_DTO_FIELD_REQUIRE(String, dayTimes, ZH_WORDS_GETTER("frequency.field.day-times"), false);
	// Number of days in the cycle.
	API_DTO_FIELD_REQUIRE(Int32, dayCount, ZH_WORDS_GETTER("frequency.field.day-count"), false);
	// Numeric execution period.
	API_DTO_FIELD_REQUIRE(Int32, executionPeriod, ZH_WORDS_GETTER("frequency.field.execution-period"), false);
	// Execution period unit.
	API_DTO_FIELD_REQUIRE(String, executionPeriodUnit, ZH_WORDS_GETTER("frequency.field.execution-period-unit"), false);
	// Interval between execution days.
	API_DTO_FIELD_REQUIRE(Int32, dayInterval, ZH_WORDS_GETTER("frequency.field.day-interval"), false);
	// Maximum total execution count.
	API_DTO_FIELD_REQUIRE(Int32, totalExecutionCount, ZH_WORDS_GETTER("frequency.field.total-execution-count"), false);
	// Applicable clinical scenes.
	API_DTO_FIELD_REQUIRE(String, applicableScenes, ZH_WORDS_GETTER("frequency.field.applicable-scenes"), false);
	// Cross-day strategy code.
	API_DTO_FIELD_REQUIRE(String, allowCrossDay, ZH_WORDS_GETTER("frequency.field.allow-cross-day"), true);
	// Display order.
	API_DTO_FIELD_REQUIRE(Int32, sortNo, ZH_WORDS_GETTER("frequency.field.sort-no"), true);
};

class FrequencyPageDTO : public PageDTO<FrequencyDTO::Wrapper>
{
	DTO_INIT(FrequencyPageDTO, PageDTO<FrequencyDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYDTO_H_
