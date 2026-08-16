/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/25 00:00:00

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
#include "MedicalOrderExecutionController.h"
#include "service/medicalorderexecution/MedicalOrderExecutionService.h"
#include "dao/common/NurseStationAccessDAO.h"

MedicalOrderPageJsonVO::Wrapper MedicalOrderExecutionController::execQueryExecutionItems(
	const MedicalOrderQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto vo = MedicalOrderPageJsonVO::createShared();
	if (!query || !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize)
		|| !inpatientnurse::validation::present(query->encounterId)
		|| !inpatientnurse::validation::validDateRange(query->startDate, query->endDate)
		|| !inpatientnurse::validation::oneOf(query->statusEnum,
			{"DRAFT", "ACTIVE", "VERIFIED", "IN_PROGRESS", "COMPLETED",
			 "CANCELLED", "REJECTED", "RETURNED", "STOPPED"})
		|| !inpatientnurse::validation::oneOf(query->categoryEnum,
			{"MEDICATION", "NURSE_PRICING", "PROCEDURE", "LAB", "EXAM",
			 "SERVICE", "DEVICE"})
		|| !inpatientnurse::validation::oneOf(query->priorityEnum,
			{"ROUTINE", "URGENT", "STAT"})
		|| !inpatientnurse::validation::oneOf(query->intentEnum,
			{"LONG_TERM", "TEMPORARY"})
		|| !inpatientnurse::validation::oneOf(query->therapyEnum,
			{"LONG_TERM", "TEMPORARY"}))
    {
        vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessEncounter(
		payload.getId(), query->encounterId.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
    return MedicalOrderExecutionService().queryExecutionItems(query);
}

StringJsonVO::Wrapper MedicalOrderExecutionController::execStartExecution(
    const ExecuteOrderDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto vo = StringJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->serviceRequestId)
		|| !inpatientnurse::validation::present(dto->encounterId)
		|| !inpatientnurse::validation::validDateTime(dto->planTime, false)
		|| !inpatientnurse::validation::oneOf(
			dto->orderType, {"SERVICE_REQUEST", "MED_REQUEST"}, false))
    {
        vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	if (!NurseStationAccessDAO().canAccessEncounter(
		payload.getId(), dto->encounterId.getValue("")))
	{
		vo->init(nullptr, RS_UNAUTHORIZED);
		return vo;
	}
	dto->performerId = payload.getId();
    return MedicalOrderExecutionService().execStartExecution(dto);
}

OrderExecutionWorkbenchJsonVO::Wrapper
MedicalOrderExecutionController::execQueryExecutionWorkbench(
	const OrderExecutionWorkbenchQueryDTO::Wrapper& dto,
	const PayloadDTO& payload)
{
	auto vo = OrderExecutionWorkbenchJsonVO::createShared();
	if (!dto || !dto->encounterIds || dto->encounterIds->empty()
		|| dto->encounterIds->size() > 50
		|| !inpatientnurse::validation::oneOf(dto->executeStatus,
			{"PENDING", "EXECUTED", "SKIPPED", "CANCELLED"}, false)
		|| !inpatientnurse::validation::oneOf(dto->therapyType,
			{"ALL", "LONG_TERM", "TEMPORARY"}, false)
		|| !inpatientnurse::validation::oneOf(dto->medicineCategory,
			{"WEST", "TCM"}, false)
		|| !inpatientnurse::validation::validDateTime(dto->executionDeadline, false)) {
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> unique;
	for (const auto& encounterId : *dto->encounterIds) {
		if (!inpatientnurse::validation::present(encounterId)
			|| !unique.insert(encounterId.getValue("")).second) {
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
		if (!NurseStationAccessDAO().canAccessEncounter(
			payload.getId(), encounterId.getValue(""))) {
			vo->init(nullptr, RS_UNAUTHORIZED);
			return vo;
		}
	}
	vo->success(MedicalOrderExecutionService().queryExecutionWorkbench(dto));
	return vo;
}

BatchOrderExecutionJsonVO::Wrapper MedicalOrderExecutionController::execExecuteOrderBatch(
	const BatchExecuteOrderRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto vo = BatchOrderExecutionJsonVO::createShared();
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) {
		vo->init(nullptr, RS_PARAMS_INVALID);
		return vo;
	}
	std::set<std::string> unique;
	for (const auto& item : *dto->items) {
		if (!item || !inpatientnurse::validation::present(item->orderId)
			|| !inpatientnurse::validation::oneOf(
				item->orderType, {"MED_REQUEST", "SERVICE_REQUEST"}, false)
			|| !inpatientnurse::validation::validDateTime(item->planTime, false)
			|| !inpatientnurse::validation::validDateTime(item->actualTime, false)
			|| !inpatientnurse::validation::present(item->encounterId)
			|| !inpatientnurse::validation::present(item->accountId)
				|| (item->prescriptionNo && item->prescriptionNo->size() > 255)
				|| (item->lineRequestIds && item->lineRequestIds->size() > 100)
				|| (item->lineRequestIds && item->lineRequestIds->size() > 1
					&& !inpatientnurse::validation::present(item->prescriptionNo))
			|| (item->bindItems && item->bindItems->size() > 100)
			|| !unique.insert(item->orderType.getValue("") + ":"
				+ item->orderId.getValue("") + ":" + item->planTime.getValue("")).second) {
			vo->init(nullptr, RS_PARAMS_INVALID);
			return vo;
		}
		if (!NurseStationAccessDAO().canAccessEncounter(
			payload.getId(), item->encounterId.getValue(""))) {
			vo->init(nullptr, RS_UNAUTHORIZED);
			return vo;
		}
		if (item->lineRequestIds) {
			std::set<std::string> lineIds;
			for (const auto& requestId : *item->lineRequestIds)
				if (!inpatientnurse::validation::present(requestId)
					|| !lineIds.insert(requestId.getValue("")).second) {
					vo->init(nullptr, RS_PARAMS_INVALID);
					return vo;
				}
		}
		if (item->bindItems) {
			std::set<std::string> bindingIds;
			for (const auto& binding : *item->bindItems)
				if (!binding || !inpatientnurse::validation::present(binding->bindingId)
					|| binding->quantity.getValue(0) <= 0
					|| !bindingIds.insert(binding->bindingId.getValue("")).second) {
					vo->init(nullptr, RS_PARAMS_INVALID);
					return vo;
				}
		}
	}
	auto result = MedicalOrderExecutionService().executeBatch(dto, payload);
	result->success ? vo->success(result) : vo->fail(result);
	return vo;
}
