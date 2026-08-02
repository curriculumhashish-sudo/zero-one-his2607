/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
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
#include "MedicationCollectionController.h"
#include "service/medicationcollection/MedicationCollectionService.h"

PagePendingDispenseSummaryLinesJsonVO::Wrapper
MedicationCollectionController::execQuerySummaryOrderItems(
	const PagePendingDispenseSummaryLinesQuery::Wrapper& query)
{
	auto vo=PagePendingDispenseSummaryLinesJsonVO::createShared();if(!query||!query->encounterIds||query->encounterIds->empty()||query->pageIndex.getValue(0)<1||query->pageSize.getValue(0)<1){vo->init(nullptr,RS_PARAMS_INVALID);return vo;}vo->success(MedicationCollectionService().listPending(query));return vo;
}

CreateMedicationSummaryJsonVO::Wrapper MedicationCollectionController::execSummaryMedication(
	const CreateMedicationSummaryDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto vo=CreateMedicationSummaryJsonVO::createShared();auto result=MedicationCollectionService().summarize(dto,payload.getUsername());result->summaryNos&&!result->summaryNos->empty()?vo->success(result):vo->fail(result);return vo;
}

SubmitSummaryJsonVO::Wrapper MedicationCollectionController::execSubmitSummary(
	const SubmitSummaryRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto jvo = SubmitSummaryJsonVO::createShared();
	if (!dto || !dto->summaryNos || dto->summaryNos->empty() || dto->summaryNos->size() > 50)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	for (const auto& summaryNo : *dto->summaryNos)
	{
		if (!summaryNo || summaryNo->empty())
		{
			jvo->init(nullptr, RS_PARAMS_INVALID);
			return jvo;
		}
	}
	try
	{
		auto result = MedicationCollectionService().submitSummary(dto, payload.getUsername());
		result && result->summaryNos && !result->summaryNos->empty() ? jvo->success(result) : jvo->fail(result);
	}
	catch (const std::exception&)
	{
		jvo->init(nullptr, RS_FAIL);
	}
	return jvo;
}

CancelSummaryJsonVO::Wrapper MedicationCollectionController::execCancelSummary(
	const CancelSummaryRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto jvo = CancelSummaryJsonVO::createShared();
	if (!dto || !dto->summaryNos || dto->summaryNos->empty() || dto->summaryNos->size() > 50)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	for (const auto& summaryNo : *dto->summaryNos)
	{
		if (!summaryNo || summaryNo->empty())
		{
			jvo->init(nullptr, RS_PARAMS_INVALID);
			return jvo;
		}
	}
	try
	{
		auto result = MedicationCollectionService().cancelSummary(dto, payload.getUsername());
		result && result->summaryNos && !result->summaryNos->empty() ? jvo->success(result) : jvo->fail(result);
	}
	catch (const std::exception&)
	{
		jvo->init(nullptr, RS_FAIL);
	}
	return jvo;
}
