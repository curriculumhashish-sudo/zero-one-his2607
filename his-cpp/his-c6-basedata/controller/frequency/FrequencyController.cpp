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
#include "stdafx.h"
#include "FrequencyController.h"

namespace
{
	// Build one sample frequency row for the Swagger example response.
	FrequencyDTO::Wrapper buildSampleFrequencyRow(
		const String& id, const String& code, const String& name,
		const String& cycleType, const String& dayTimes, const String& allowCrossDay, Int32 sortNo)
	{
		auto row = FrequencyDTO::createShared();
		row->id = id;
		row->code = code;
		row->name = name;
		row->status = 1;
		row->cycleType = cycleType;
		row->dayTimes = dayTimes;
		row->dayCount = nullptr;
		row->executionPeriod = nullptr;
		row->executionPeriodUnit = nullptr;
		row->dayInterval = nullptr;
		row->totalExecutionCount = nullptr;
		row->applicableScenes = u8"全部";
		row->allowCrossDay = allowCrossDay;
		row->sortNo = sortNo;
		return row;
	}
}

FrequencyPageJsonVO::Wrapper FrequencyController::execQueryAll(const FrequencyQuery::Wrapper& query)
{
	// This stage only defines the domain model and Swagger contract; no Service/DAO call yet.
	// The example rows below mirror the first entries of the "Frequency Management" list page.
	auto pageDto = FrequencyPageDTO::createShared();
	pageDto->addData(buildSampleFrequencyRow("1", "QD", u8"每日一次", "daily", "08:00", "same-day", 10));
	pageDto->addData(buildSampleFrequencyRow("2", "BID", u8"每日两次", "daily", "08:00,18:00", "same-day", 20));

	auto oneWithInterval = FrequencyDTO::createShared();
	oneWithInterval->id = "10";
	oneWithInterval->code = "QOD";
	oneWithInterval->name = u8"隔日一次";
	oneWithInterval->status = 1;
	oneWithInterval->cycleType = "daily";
	oneWithInterval->dayTimes = "08:00";
	oneWithInterval->dayCount = nullptr;
	oneWithInterval->executionPeriod = nullptr;
	oneWithInterval->executionPeriodUnit = nullptr;
	oneWithInterval->dayInterval = 2;
	oneWithInterval->totalExecutionCount = nullptr;
	oneWithInterval->applicableScenes = u8"全部";
	oneWithInterval->allowCrossDay = "same-day";
	oneWithInterval->sortNo = 100;
	pageDto->addData(oneWithInterval);

	auto q12h = FrequencyDTO::createShared();
	q12h->id = "14";
	q12h->code = "Q12H";
	q12h->name = u8"每12小时一次";
	q12h->status = 1;
	q12h->cycleType = "interval";
	q12h->dayTimes = nullptr;
	q12h->dayCount = nullptr;
	q12h->executionPeriod = 12;
	q12h->executionPeriodUnit = u8"小时";
	q12h->dayInterval = nullptr;
	q12h->totalExecutionCount = nullptr;
	q12h->applicableScenes = u8"全部";
	q12h->allowCrossDay = "cross-day";
	q12h->sortNo = 140;
	pageDto->addData(q12h);

	pageDto->pageIndex = query->pageIndex;
	pageDto->pageSize = query->pageSize;
	pageDto->total = 15;
	pageDto->calcPages();

	auto vo = FrequencyPageJsonVO::createShared();
	vo->success(pageDto);
	return vo;
}
