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
#include "stdafx.h"
#include "FrequencyController.h"

namespace
{
	FrequencyDTO::Wrapper buildSampleFrequencyRow(
		const String& id, const String& code, const String& name,
		const String& cycleType, const String& dayTimes,
		const String& allowCrossDay, Int32 sortNo)
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
		row->applicableScenes = u8"\u5168\u90e8";
		row->allowCrossDay = allowCrossDay;
		row->sortNo = sortNo;
		return row;
	}
}

ListJsonVO<FrequencyVO::Wrapper>::Wrapper FrequencyController::executeGetFrequencyName(
	const FrequencyQuery::Wrapper& query)
{
	// Preserve the branch implementation placeholder for the selection endpoint.
	return {};
}

FrequencyPageJsonVO::Wrapper FrequencyController::execQueryAll(
	const FrequencyQuery::Wrapper& query)
{
	// Return sample rows until the Service and DAO layers are implemented.
	auto pageDto = FrequencyPageDTO::createShared();
	pageDto->addData(buildSampleFrequencyRow(
		"1", "QD", u8"\u6bcf\u65e5\u4e00\u6b21", "daily", "08:00", "same-day", 10));
	pageDto->addData(buildSampleFrequencyRow(
		"2", "BID", u8"\u6bcf\u65e5\u4e24\u6b21", "daily", "08:00,18:00", "same-day", 20));

	auto oneWithInterval = FrequencyDTO::createShared();
	oneWithInterval->id = "10";
	oneWithInterval->code = "QOD";
	oneWithInterval->name = u8"\u9694\u65e5\u4e00\u6b21";
	oneWithInterval->status = 1;
	oneWithInterval->cycleType = "daily";
	oneWithInterval->dayTimes = "08:00";
	oneWithInterval->dayCount = nullptr;
	oneWithInterval->executionPeriod = nullptr;
	oneWithInterval->executionPeriodUnit = nullptr;
	oneWithInterval->dayInterval = 2;
	oneWithInterval->totalExecutionCount = nullptr;
	oneWithInterval->applicableScenes = u8"\u5168\u90e8";
	oneWithInterval->allowCrossDay = "same-day";
	oneWithInterval->sortNo = 100;
	pageDto->addData(oneWithInterval);

	auto q12h = FrequencyDTO::createShared();
	q12h->id = "14";
	q12h->code = "Q12H";
	q12h->name = u8"\u6bcf12\u5c0f\u65f6\u4e00\u6b21";
	q12h->status = 1;
	q12h->cycleType = "interval";
	q12h->dayTimes = nullptr;
	q12h->dayCount = nullptr;
	q12h->executionPeriod = 12;
	q12h->executionPeriodUnit = u8"\u5c0f\u65f6";
	q12h->dayInterval = nullptr;
	q12h->totalExecutionCount = nullptr;
	q12h->applicableScenes = u8"\u5168\u90e8";
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

FrequencyDetailJsonVO::Wrapper FrequencyController::execQueryOne(const String& id)
{
	FrequencyDTO::Wrapper dto;
	if (id == "1") {
		dto = buildSampleFrequencyRow(
			"1", "QD", u8"\u6bcf\u65e5\u4e00\u6b21", "daily", "08:00", "same-day", 10);
	} else if (id == "2") {
		dto = buildSampleFrequencyRow(
			"2", "BID", u8"\u6bcf\u65e5\u4e24\u6b21", "daily", "08:00,18:00", "same-day", 20);
	} else if (id == "10") {
		dto = FrequencyDTO::createShared();
		dto->id = "10";
		dto->code = "QOD";
		dto->name = u8"\u9694\u65e5\u4e00\u6b21";
		dto->status = 1;
		dto->cycleType = "daily";
		dto->dayTimes = "08:00";
		dto->dayInterval = 2;
		dto->applicableScenes = u8"\u5168\u90e8";
		dto->allowCrossDay = "same-day";
		dto->sortNo = 100;
	} else if (id == "14") {
		dto = FrequencyDTO::createShared();
		dto->id = "14";
		dto->code = "Q12H";
		dto->name = u8"\u6bcf12\u5c0f\u65f6\u4e00\u6b21";
		dto->status = 1;
		dto->cycleType = "interval";
		dto->executionPeriod = 12;
		dto->executionPeriodUnit = u8"\u5c0f\u65f6";
		dto->applicableScenes = u8"\u5168\u90e8";
		dto->allowCrossDay = "cross-day";
		dto->sortNo = 140;
	} else {
		dto = FrequencyDTO::createShared();
		dto->id = id;
		dto->code = "CUSTOM";
		dto->name = u8"\u793a\u4f8b\u9891\u6b21";
		dto->status = 1;
		dto->cycleType = "daily";
		dto->dayTimes = "08:00";
		dto->applicableScenes = u8"\u5168\u90e8";
		dto->allowCrossDay = "same-day";
		dto->sortNo = 999;
	}

	auto vo = FrequencyDetailJsonVO::createShared();
	vo->success(dto);
	return vo;
}
