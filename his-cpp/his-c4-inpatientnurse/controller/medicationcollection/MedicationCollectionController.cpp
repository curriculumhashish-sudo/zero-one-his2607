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
#include "dao/common/NurseStationAccessDAO.h"

namespace
{
	bool canAccessEncounters(const PayloadDTO& payload, const oatpp::String& encounterIds)
	{
		std::stringstream stream(encounterIds.getValue(""));
		std::string encounterId;
		NurseStationAccessDAO access;
		while (std::getline(stream, encounterId, ','))
		{
			encounterId = inpatientnurse::validation::trim(encounterId);
			if (!access.canAccessEncounter(payload.getId(), encounterId)) return false;
		}
		return true;
	}
}

PagePendingDispenseSummaryLinesJsonVO::Wrapper
MedicationCollectionController::execQuerySummaryOrderItems(
	const PagePendingDispenseSummaryLinesQuery::Wrapper& query,
	const PayloadDTO& payload)
{
	auto vo = PagePendingDispenseSummaryLinesJsonVO::createShared();
	if (!query || !inpatientnurse::validation::validCsv(query->encounterIds, 50, true)
		|| !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize)
		|| !inpatientnurse::validation::oneOf(query->medicineCategoryTab, {"all", "west", "tcm"}, false)
		|| !inpatientnurse::validation::oneOf(query->therapyFilter, {"all", "LONG_TERM", "TEMPORARY"}, false)
		|| !inpatientnurse::validation::validDateTime(query->queryDeadline)
		|| !inpatientnurse::validation::validDateRange(query->startDate, query->endDate)
		|| !inpatientnurse::validation::oneOf(query->sortDirection, {"ASC", "DESC"}))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!canAccessEncounters(payload, query->encounterIds))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	vo->success(MedicationCollectionService().listPending(query));
	return vo;
}

PagePendingDispenseSummaryLinesJsonVO::Wrapper
MedicationCollectionController::execQuerySummaryOrderItemsByBody(
	const MedicationSummaryWorkbenchQueryDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto vo = PagePendingDispenseSummaryLinesJsonVO::createShared();
	if (!dto || !dto->encounterIds || dto->encounterIds->empty()
		|| dto->encounterIds->size() > 50
		|| !inpatientnurse::validation::validPage(dto->pageIndex, dto->pageSize)
		|| !inpatientnurse::validation::oneOf(
			dto->medicineCategoryTab, {"all", "west", "tcm"}, false)
		|| !inpatientnurse::validation::oneOf(
			dto->therapyFilter, {"all", "LONG_TERM", "TEMPORARY"}, false)
		|| !inpatientnurse::validation::validDateTime(dto->queryDeadline)
		|| !inpatientnurse::validation::validDateRange(dto->startDate, dto->endDate)
		|| !inpatientnurse::validation::oneOf(dto->sortDirection, {"ASC", "DESC"}))
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> unique;
	std::stringstream encounterCsv;
	NurseStationAccessDAO access;
	for (const auto& value : *dto->encounterIds)
	{
		const auto encounterId = inpatientnurse::validation::trim(value.getValue(""));
		if (encounterId.empty() || !unique.insert(encounterId).second)
		{
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
		if (!access.canAccessEncounter(payload.getId(), encounterId))
		{
			vo->init(nullptr, RS_UNAUTHORIZED);
			return vo;
		}
		if (encounterCsv.tellp() > 0) encounterCsv << ',';
		encounterCsv << encounterId;
	}
	auto query = PagePendingDispenseSummaryLinesQuery::createShared();
	query->pageIndex = dto->pageIndex;
	query->pageSize = dto->pageSize;
	query->encounterIds = encounterCsv.str();
	query->medicineCategoryTab = dto->medicineCategoryTab;
	query->therapyFilter = dto->therapyFilter;
	query->queryDeadline = dto->queryDeadline;
	query->keyword = dto->keyword;
	query->startDate = dto->startDate;
	query->endDate = dto->endDate;
	query->sortBy = dto->sortBy;
	query->sortDirection = dto->sortDirection;
	query->where = dto->where;
	vo->success(MedicationCollectionService().listPending(query));
	return vo;
}

CreateMedicationSummaryJsonVO::Wrapper MedicationCollectionController::execSummaryMedication(
	const CreateMedicationSummaryDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto vo = CreateMedicationSummaryJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !dto->dispenseItems || dto->dispenseItems->empty()
		|| dto->dispenseItems->size() > 500)
	{
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> dispenseIds;
	for (const auto& item : *dto->dispenseItems)
	{
		if (!item || !inpatientnurse::validation::present(item->dispenseId)
			|| !inpatientnurse::validation::present(item->encounterId)
			|| !dispenseIds.insert(
				inpatientnurse::validation::trim(item->dispenseId->c_str())).second)
		{
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
		if (!NurseStationAccessDAO().canAccessEncounter(
			payload.getId(), item->encounterId.getValue("")))
		{
			vo->init(nullptr, RS_UNAUTHORIZED);
			return vo;
		}
	}
	auto result = MedicationCollectionService().summarize(dto, payload.getId());
	result->summaryNos && !result->summaryNos->empty() ? vo->success(result) : vo->fail(result);
	return vo;
}

SubmitSummaryJsonVO::Wrapper MedicationCollectionController::execSubmitSummary(
	const SubmitSummaryRequestDTO::Wrapper& dto,const PayloadDTO& payload)
{
	auto jvo = SubmitSummaryJsonVO::createShared();
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty()
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	std::set<std::string> summaryNos;
	for (const auto& item : *dto->items)
	{
		if (!item || !inpatientnurse::validation::present(item->summaryNo)
			|| !item->version.getPtr() || item->version.getValue(-1) < 0
			|| !summaryNos.insert(
				inpatientnurse::validation::trim(item->summaryNo->c_str())).second)
		{
			jvo->init(nullptr, RS_PARAMS_INVALID);
			return jvo;
		}
		NurseStationAccessDAO access;
		if (!access.canAccessMedicationSummary(
			payload.getId(), item->summaryNo.getValue(""))
			&& !access.ownsMedicationIdempotency(payload.getId(),
				dto->idempotencyKey.getValue(""), "SUBMIT"))
		{
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
	try
	{
		auto result = MedicationCollectionService().submitSummary(dto, payload.getId());
		jvo->success(result);
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
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty()
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50)
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	std::set<std::string> summaryNos;
	for (const auto& item : *dto->items)
	{
		if (!item || !inpatientnurse::validation::present(item->summaryNo)
			|| !item->version.getPtr() || item->version.getValue(-1) < 0
			|| !summaryNos.insert(
				inpatientnurse::validation::trim(item->summaryNo->c_str())).second)
		{
			jvo->init(nullptr, RS_PARAMS_INVALID);
			return jvo;
		}
		NurseStationAccessDAO access;
		if (!access.canAccessMedicationSummary(
			payload.getId(), item->summaryNo.getValue(""))
			&& !access.ownsMedicationIdempotency(payload.getId(),
				dto->idempotencyKey.getValue(""), "CANCEL"))
		{
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
	try
	{
		auto result = MedicationCollectionService().cancelSummary(dto, payload.getId());
		jvo->success(result);
	}
	catch (const std::exception&)
	{
		jvo->init(nullptr, RS_FAIL);
	}
	return jvo;
}
