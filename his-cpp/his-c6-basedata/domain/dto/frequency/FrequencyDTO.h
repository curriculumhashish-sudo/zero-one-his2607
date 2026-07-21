#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: AI Assistant
 @Date: 2026/07/20 00:00:00

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

/**
 * 频次列表行DTO
 */
class FrequencyDTO : public oatpp::DTO
{
	DTO_INIT(FrequencyDTO, DTO);

	// 频次主键
	API_DTO_FIELD_REQUIRE(String, id, ZH_WORDS_GETTER("frequency.field.id"), true);
	// 频次编码
	API_DTO_FIELD_REQUIRE(String, code, ZH_WORDS_GETTER("frequency.field.code"), true);
	// 频次名称
	API_DTO_FIELD_REQUIRE(String, name, ZH_WORDS_GETTER("frequency.field.name"), true);
	// 启用/停用状态 1-启用 0-停用
	API_DTO_FIELD_REQUIRE(Int32, status, ZH_WORDS_GETTER("frequency.field.status"), true);
	// 周期类型编码：daily/weekly/interval/as-needed/fixed-count
	API_DTO_FIELD_REQUIRE(String, cycleType, ZH_WORDS_GETTER("frequency.field.cycle-type"), true);
	// 日内执行时点，逗号分隔，如"08:00,18:00"
	API_DTO_FIELD_REQUIRE(String, dayTimes, ZH_WORDS_GETTER("frequency.field.day-times"), false);
	// 日计数
	API_DTO_FIELD_REQUIRE(Int32, dayCount, ZH_WORDS_GETTER("frequency.field.day-count"), false);
	// 执行周期数值，配合执行周期单位使用，如12+小时
	API_DTO_FIELD_REQUIRE(Int32, executionPeriod, ZH_WORDS_GETTER("frequency.field.execution-period"), false);
	// 执行周期单位
	API_DTO_FIELD_REQUIRE(String, executionPeriodUnit, ZH_WORDS_GETTER("frequency.field.execution-period-unit"), false);
	// 日间隔，如隔日一次为2
	API_DTO_FIELD_REQUIRE(Int32, dayInterval, ZH_WORDS_GETTER("frequency.field.day-interval"), false);
	// 总执行次数上限
	API_DTO_FIELD_REQUIRE(Int32, totalExecutionCount, ZH_WORDS_GETTER("frequency.field.total-execution-count"), false);
	// 适用场景，如"全部"/"门诊、住院"/"住院"
	API_DTO_FIELD_REQUIRE(String, applicableScenes, ZH_WORDS_GETTER("frequency.field.applicable-scenes"), false);
	// 跨日策略编码：same-day/cross-day/by-shift
	API_DTO_FIELD_REQUIRE(String, allowCrossDay, ZH_WORDS_GETTER("frequency.field.allow-cross-day"), true);
	// 排序号
	API_DTO_FIELD_REQUIRE(Int32, sortNo, ZH_WORDS_GETTER("frequency.field.sort-no"), true);
};

/**
 * 频次分页列表响应DTO
 */
class FrequencyPageDTO : public PageDTO<FrequencyDTO::Wrapper>
{
	DTO_INIT(FrequencyPageDTO, PageDTO<FrequencyDTO::Wrapper>);
};

#include OATPP_CODEGEN_END(DTO)

#endif // !_FREQUENCYDTO_H_
