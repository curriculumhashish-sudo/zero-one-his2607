/*
 Copyright Zero One Star. All rights reserved.

 @Author: Swan_Woo
 @Date: 2026/07/20 16:00:00

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
#include "MedicalOrderExecutionService.h"
#include "dao/medicalorderexecution/MedicalOrderExecutionDAO.h"
#include "id/UuidFacade.h"

#include <algorithm>
#include <iomanip>
#include <set>
#include <sstream>

namespace
{
std::string executionItemKey(const BatchExecuteOrderItemDTO::Wrapper& item)
{
	return item->orderType.getValue("") + ":" + item->orderId.getValue("")
		+ ":" + item->planTime.getValue("");
}

std::string buildExecutionFingerprint(const BatchExecuteOrderRequestDTO::Wrapper& dto)
{
	std::ostringstream stream;
	for (const auto& item : *dto->items)
	{
		stream << executionItemKey(item) << '|' << item->encounterId.getValue("")
			<< '|' << item->accountId.getValue("") << '|'
			<< item->actualTime.getValue("") << '|'
			<< item->prescriptionNo.getValue("");
		if (item->lineRequestIds)
			for (const auto& requestId : *item->lineRequestIds)
				stream << "|L:" << requestId.getValue("");
		if (item->bindItems)
			for (const auto& binding : *item->bindItems)
				stream << "|B:" << binding->bindingId.getValue("") << ':'
					<< std::fixed << std::setprecision(4)
					<< binding->quantity.getValue(0);
		stream << '\n';
	}
	return stream.str();
}

bool restoreExecutionResult(const std::string& stored,
	const std::string& fingerprint,
	const BatchOrderExecutionResponseDTO::Wrapper& response)
{
	const std::string marker = "\n--RESULT--\n";
	const auto markerPosition = stored.find(marker);
	if (markerPosition == std::string::npos
		|| stored.substr(0, markerPosition) != fingerprint) return false;
	std::stringstream lines(stored.substr(markerPosition + marker.size()));
	std::string line;
	while (std::getline(lines, line))
	{
		const auto tab = line.find('\t');
		if (tab == std::string::npos) continue;
		auto result = BatchOrderExecutionResultItemDTO::createShared();
		result->itemKey = line.substr(0, tab).c_str();
		result->procedureId = line.substr(tab + 1).c_str();
		result->success = true;
		result->message = ZH_WORDS_GETTER("business-text.execution.idempotent-success");
		response->results->push_back(result);
		response->processedCount = response->processedCount + 1;
	}
	response->success = !response->results->empty();
	return response->success.getValue(false);
}

std::vector<BatchExecuteOrderItemDTO::Wrapper> expandExecutionItems(
	const BatchExecuteOrderRequestDTO::Wrapper& dto)
{
	std::vector<BatchExecuteOrderItemDTO::Wrapper> result;
	std::set<std::string> unique;
	for (const auto& source : *dto->items)
	{
		std::vector<std::string> requestIds;
		if (source->orderType == "MED_REQUEST" && source->lineRequestIds
			&& !source->lineRequestIds->empty())
		{
			for (const auto& requestId : *source->lineRequestIds)
				requestIds.push_back(requestId.getValue(""));
		}
		else requestIds.push_back(source->orderId.getValue(""));
		if (std::find(requestIds.begin(), requestIds.end(),
			source->orderId.getValue("")) == requestIds.end())
			throw std::runtime_error("TCM_LINE_REQUEST_MISMATCH");
		for (const auto& requestId : requestIds)
		{
			auto item = BatchExecuteOrderItemDTO::createShared();
			item->orderId = requestId.c_str();
			item->orderType = source->orderType;
			item->planTime = source->planTime;
			item->encounterId = source->encounterId;
			item->accountId = source->accountId;
			item->actualTime = source->actualTime;
			// A TCM prescription is expanded to medication lines, but the optional
			// service/device bindings belong to the selected prescription row only.
			if (requestId == source->orderId.getValue(""))
				item->bindItems = source->bindItems;
			else
				item->bindItems = oatpp::List<OrderExecutionChargeBindingDTO::Wrapper>::createShared();
			item->prescriptionNo = source->prescriptionNo;
			const auto key = executionItemKey(item);
			if (!unique.insert(key).second)
				throw std::runtime_error("DUPLICATE_EXECUTION_SLOT");
			result.push_back(item);
		}
	}
	return result;
}

std::vector<OrderExecutionChargeBindingDTO::Wrapper> resolveExecutionBindings(
	MedicalOrderExecutionDAO& dao, const BatchExecuteOrderItemDTO::Wrapper& item)
{
	std::vector<OrderExecutionChargeBindingDTO::Wrapper> result;
	if (!item->bindItems || item->bindItems->empty()) return result;
	std::map<std::string, OrderExecutionChargeBindingDTO::Wrapper> allowed;
	for (const auto& binding : dao.selectExecutionBindings(
		item->orderType.getValue(""), item->orderId.getValue("")))
		allowed[binding->bindingId.getValue("")] = binding;
	std::set<std::string> unique;
	for (const auto& requested : *item->bindItems)
	{
		if (!requested || !requested->bindingId || requested->bindingId->empty()
			|| requested->quantity.getValue(0) <= 0
			|| !unique.insert(requested->bindingId.getValue("")).second)
			throw std::runtime_error("INVALID_EXECUTION_BINDING");
		const auto found = allowed.find(requested->bindingId.getValue(""));
		if (found == allowed.end())
			throw std::runtime_error("INVALID_EXECUTION_BINDING");
		auto canonical = found->second;
		canonical->quantity = requested->quantity;
		result.push_back(canonical);
	}
	return result;
}
}

// ============================================================================
// Helper: convert DO to DTO (extract only fields needed by frontend).
// DO has 50+ columns matching the DB table; DTO exposes ~20 selected fields.
// ============================================================================
static MedicalOrderItemDTO::Wrapper doToDto(const PtrMedicalOrderExecutionRow& row)
{
    const auto& sr = row->order;
    auto vo = MedicalOrderItemDTO::createShared();
    // Basic
    vo->id = sr->getId();
    vo->orderType = row->orderType.c_str();
    vo->busNo = sr->getBusNo();
    vo->prescriptionNo = sr->getPrescriptionNo();
    // Status
    vo->statusEnum = sr->getStatusEnum();
	vo->intentEnum = sr->getTherapyEnum();
	vo->therapyEnum = sr->getTherapyEnum();
    vo->categoryEnum = sr->getCategoryEnum();
    vo->priorityEnum = sr->getPriorityEnum();
    // Patient
    vo->patientId = sr->getPatientId();
    vo->encounterId = sr->getEncounterId();
    vo->authoredTime = sr->getAuthoredTime();
	vo->planTime = sr->getOccurrenceStartTime();
	vo->adviceDisplayName = sr->getContentJson();
	vo->procedureId = row->procedureId.c_str();
	vo->listRowKey = row->orderType + ":" + sr->getId();
	vo->specificationText = row->specificationText.c_str();
	vo->dosageDisplay = row->dosageDisplay.c_str();
	vo->usageDisplay = row->usageDisplay.c_str();
	vo->frequencyDisplay = row->frequencyDisplay.c_str();
	vo->stopTime = row->stopTime.c_str();
	vo->requesterName = row->requesterName.c_str();
	vo->performerLocationDisplay = row->locationName.c_str();
	vo->tcmFlag = row->tcmFlag;
	vo->chineseHerbsDoseQuantity = row->chineseHerbsDoseQuantity;
	vo->lineRequestIds->push_back(sr->getId().c_str());
	vo->therapyDisplay = sr->getTherapyEnum() == "LONG_TERM"
		? ZH_WORDS_GETTER("business-text.enum.therapy-long-term")
		: ZH_WORDS_GETTER("business-text.enum.therapy-temporary");
	auto slot = OrderExecutionSlotDTO::createShared();
	slot->planTime = sr->getOccurrenceStartTime();
	slot->procedureId = row->procedureId.c_str();
	slot->statusEnum = sr->getStatusEnum();
	slot->recordedTime = row->recordedTime.c_str();
	if (sr->getStatusEnum() == "COMPLETED") vo->executedSlots->push_back(slot);
	else if (sr->getStatusEnum() == "STOPPED") vo->skippedSlots->push_back(slot);
	else if (sr->getStatusEnum() == "CANCEL") vo->cancelledSlots->push_back(slot);
	else vo->pendingSlots->push_back(slot);
    // Personnel
    vo->requesterId = sr->getRequesterId();
    vo->performerId = sr->getPerformerId();
    vo->performerCheckId = sr->getPerformerCheckId();
    vo->checkTime = sr->getCheckTime();
    // Location
    vo->locationId = sr->getLocationId();
    // Content
    vo->quantity = sr->getQuantity();
    vo->unitCode = sr->getUnitCode();
    vo->executionCount = sr->getExecutionCount();
    // Notes
    vo->remark = sr->getRemark();
    vo->entrustContent = sr->getEntrustContent();
	vo->reasonText = sr->getReasonText();
	for (const auto& binding : MedicalOrderExecutionDAO().selectExecutionBindings(
		row->orderType, sr->getId()))
	{
		vo->bindingItems->push_back(binding);
		vo->bindItems->push_back(binding);
	}
    return vo;
}

// ============================================================================
// Requirement 1: get verification items
// ============================================================================
MedicalOrderPageJsonVO::Wrapper MedicalOrderExecutionService::queryVerificationItems(
    const MedicalOrderQuery::Wrapper& query)
{
    auto page = PageDTO<MedicalOrderItemDTO::Wrapper>::createShared();
    MedicalOrderExecutionDAO dao;

    // Default to pending_verification status
    if (!query->statusEnum) {
        query->statusEnum = "ACTIVE";
    }

    auto doList = dao.queryVerificationItems(query);
    auto total = dao.countVerificationItems(query);

    for (const auto& sr : doList) {
        page->addData(doToDto(sr));
    }

    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(static_cast<int64_t>(total));
    page->calcPages();

    auto result = MedicalOrderPageJsonVO::createShared();
    result->success(page);
    return result;
}

// ============================================================================
// Requirement 2: verify pass
// ============================================================================
StringJsonVO::Wrapper MedicalOrderExecutionService::execVerifyPass(
    const VerifyPassDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    if (!dto->serviceRequestId || dto->serviceRequestId.getValue("").empty()) {
        result->fail(String("serviceRequestId is required"));
        return result;
    }
    if (!dto->verifierId || dto->verifierId.getValue("").empty()) {
        result->fail(String("verifierId is required"));
        return result;
    }

    MedicalOrderExecutionDAO dao;
    int rows = dao.verifyPass(
        dto->serviceRequestId.getValue(""),
        dto->verifierId.getValue(""));

    if (rows > 0) {
        result->success(String("Verification passed"));
    } else {
        result->fail(String("Verification failed: order not found or already processed"));
    }
    return result;
}

// ============================================================================
// Requirement 3: return order
// ============================================================================
StringJsonVO::Wrapper MedicalOrderExecutionService::execReturnOrder(
    const ReturnOrderDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    if (!dto->serviceRequestId || dto->serviceRequestId.getValue("").empty()) {
        result->fail(String("serviceRequestId is required"));
        return result;
    }
    if (!dto->returnReason || dto->returnReason.getValue("").empty()) {
        result->fail(String("returnReason is required"));
        return result;
    }

    MedicalOrderExecutionDAO dao;
    int rows = dao.returnOrder(
        dto->serviceRequestId.getValue(""),
        dto->returnReason.getValue(""));

    if (rows > 0) {
        result->success(String("Order returned"));
    } else {
        result->fail(String("Return failed: order not found or already processed"));
    }
    return result;
}

// ============================================================================
// Requirement 4: get execution items
// ============================================================================
MedicalOrderPageJsonVO::Wrapper MedicalOrderExecutionService::queryExecutionItems(
    const MedicalOrderQuery::Wrapper& query)
{
    auto page = PageDTO<MedicalOrderItemDTO::Wrapper>::createShared();
    MedicalOrderExecutionDAO dao;

    // Default to verified status (ready to execute)
    if (!query->statusEnum) {
        query->statusEnum = "VERIFIED";
    }

    auto doList = dao.queryExecutionItems(query);
    auto total = dao.countExecutionItems(query);

    for (const auto& sr : doList) {
        page->addData(doToDto(sr));
    }

    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(static_cast<int64_t>(total));
    page->calcPages();

    auto result = MedicalOrderPageJsonVO::createShared();
    result->success(page);
    return result;
}

OrderExecutionWorkbenchDTO::Wrapper MedicalOrderExecutionService::queryExecutionWorkbench(
	const OrderExecutionWorkbenchQueryDTO::Wrapper& query)
{
	auto response = OrderExecutionWorkbenchDTO::createShared();
	std::map<std::string, OrderExecutionGroupDTO::Wrapper> groups;
	for (const auto& encounterId : *query->encounterIds) {
		auto group = OrderExecutionGroupDTO::createShared();
		group->encounterId = encounterId;
		groups[encounterId.getValue("")] = group;
		response->groups->push_back(group);
	}
	for (const auto& row : MedicalOrderExecutionDAO().queryExecutionWorkbenchItems(query)) {
		auto item = doToDto(row);
		if (query->executeStatus == "CANCELLED") {
			item->statusEnum = "CANCEL";
			item->pendingSlots = oatpp::List<OrderExecutionSlotDTO::Wrapper>::createShared();
			item->executedSlots = oatpp::List<OrderExecutionSlotDTO::Wrapper>::createShared();
			item->skippedSlots = oatpp::List<OrderExecutionSlotDTO::Wrapper>::createShared();
			item->cancelledSlots = oatpp::List<OrderExecutionSlotDTO::Wrapper>::createShared();
			auto slot = OrderExecutionSlotDTO::createShared();
			slot->planTime = item->planTime;
			slot->statusEnum = "CANCEL";
			item->cancelledSlots->push_back(slot);
		}
		if (item->statusEnum == "COMPLETED")
			item->statusDisplay = ZH_WORDS_GETTER("business-text.execution.status-executed");
		else if (item->statusEnum == "STOPPED")
			item->statusDisplay = ZH_WORDS_GETTER("business-text.execution.status-skipped");
		else if (item->statusEnum == "CANCEL")
			item->statusDisplay = ZH_WORDS_GETTER("business-text.execution.status-cancelled");
		else item->statusDisplay = ZH_WORDS_GETTER("business-text.execution.status-pending");
		auto group = groups[item->encounterId.getValue("")];
		if (group) group->orders->push_back(item);
	}
	return response;
}

BatchOrderExecutionResponseDTO::Wrapper MedicalOrderExecutionService::executeBatch(
	const BatchExecuteOrderRequestDTO::Wrapper& dto, const PayloadDTO& payload)
{
	auto response = BatchOrderExecutionResponseDTO::createShared();
	if (!dto || !dto->items || dto->items->empty()) return response;
	MedicalOrderExecutionDAO dao;
	const auto fingerprint = buildExecutionFingerprint(dto);
	const auto storedResults = dao.selectExecutionIdempotencySlots(
		dto->idempotencyKey.getValue(""));
	if (!storedResults.empty())
	{
		if (!restoreExecutionResult(storedResults.front(), fingerprint, response))
		{
			auto conflict = BatchOrderExecutionResultItemDTO::createShared();
			conflict->itemKey = dto->idempotencyKey;
			conflict->success = false;
			conflict->message = "IDEMPOTENCY_KEY_CONFLICT";
			response->results->push_back(conflict);
		}
		return response;
	}
	dao.getSqlSession()->beginTransaction();
	try {
			const auto items = expandExecutionItems(dto);
			std::map<std::string, double> projectedAccountAmounts;
			std::ostringstream mappings;
		for (const auto& item : items) {
			auto row = BatchOrderExecutionResultItemDTO::createShared();
			row->itemKey = executionItemKey(item).c_str();
			const auto existingId = dao.selectActiveCompletedProcedureId(
				item->orderType.getValue(""), item->orderId.getValue(""),
				item->encounterId.getValue(""), item->planTime.getValue(""));
			if (!existingId.empty()) {
				throw std::runtime_error("EXECUTION_SLOT_ALREADY_OCCUPIED");
			}
			ExecutionWriteContext context;
			if (!dao.loadExecutionWriteContext(item->orderType.getValue(""),
				item->orderId.getValue(""), item->encounterId.getValue(""),
				item->accountId.getValue(""), context))
				throw std::runtime_error("EXECUTION_CONTEXT_INVALID");
			if (item->prescriptionNo && !item->prescriptionNo->empty()
				&& context.prescriptionNo != item->prescriptionNo.getValue(""))
				throw std::runtime_error("TCM_PRESCRIPTION_MISMATCH");
			const auto bindings = resolveExecutionBindings(dao, item);
			double totalAmount = context.unitPrice * context.quantity;
			for (const auto& binding : bindings)
				totalAmount += binding->unitPrice.getValue(0)
					* binding->quantity.getValue(0);
			const auto accountId = item->accountId.getValue("");
			auto& projectedAmount = projectedAccountAmounts[accountId];
			projectedAmount += totalAmount;
			if (!dao.canCoverExecutionAmount(accountId, projectedAmount))
				throw std::runtime_error("ACCOUNT_BALANCE_INSUFFICIENT");
			if (!dao.canReserveMedication(context))
				throw std::runtime_error("MEDICATION_INVENTORY_INSUFFICIENT");
			const std::string procedureId = UuidFacade().genUuid();
			if (dao.insertCompletedProcedure(procedureId, item->orderType.getValue(""),
				item->orderId.getValue(""), item->encounterId.getValue(""),
				item->planTime.getValue(""), payload.getId(),
				item->actualTime.getValue("")) != 1)
				throw std::runtime_error("EXECUTION_STATE_CHANGED");
			if (dao.insertProcedurePerformer(UuidFacade().genUuid(), procedureId,
				payload.getId(), item->actualTime.getValue("")) != 1)
				throw std::runtime_error("EXECUTION_PERFORMER_WRITE_FAILED");
			std::string dispenseId;
			if (context.orderType == "MED_REQUEST")
			{
				dispenseId = UuidFacade().genUuid();
				if (dao.insertExecutionDispense(dispenseId, procedureId, context,
					item->planTime.getValue(""), payload.getId()) != 1)
					throw std::runtime_error("EXECUTION_DISPENSE_WRITE_FAILED");
			}
			if (dao.insertExecutionCharge(UuidFacade().genUuid(), procedureId,
				dispenseId, context, item->accountId.getValue(""),
				item->actualTime.getValue(""), payload.getId()) != 1)
				throw std::runtime_error("EXECUTION_CHARGE_WRITE_FAILED");
			for (const auto& binding : bindings)
				if (dao.insertExecutionBindingCharge(UuidFacade().genUuid(), procedureId,
					context, binding, item->accountId.getValue(""),
					item->actualTime.getValue(""), payload.getId()) != 1)
					throw std::runtime_error("EXECUTION_BINDING_WRITE_FAILED");
			if (dao.completeOrder(item->orderType.getValue(""),
					item->orderId.getValue(""), item->encounterId.getValue(""),
					payload.getId()) != 1)
				throw std::runtime_error("EXECUTION_STATE_CHANGED");
			row->success = true;
			row->procedureId = procedureId.c_str();
			row->message = ZH_WORDS_GETTER("business-text.execution.execute-success");
			response->results->push_back(row);
			response->processedCount = response->processedCount + 1;
			mappings << row->itemKey.getValue("") << '\t' << procedureId << '\n';
		}
		if (dao.insertExecutionIdempotency(UuidFacade().genUuid(),
			dto->idempotencyKey.getValue(""), payload.getId(), fingerprint,
			mappings.str()) != 1)
			throw std::runtime_error("IDEMPOTENCY_WRITE_FAILED");
		dao.getSqlSession()->commitTransaction();
		response->success = true;
	}
	catch (const std::exception& exception) {
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
		response->processedCount = 0;
		response->results = oatpp::List<BatchOrderExecutionResultItemDTO::Wrapper>::createShared();
		for (const auto& item : *dto->items)
		{
			auto failed = BatchOrderExecutionResultItemDTO::createShared();
			failed->itemKey = executionItemKey(item).c_str();
			failed->success = false;
			failed->message = exception.what();
			response->results->push_back(failed);
		}
	}
	return response;
}

// ============================================================================
// Requirement 5: start execution
// ============================================================================
StringJsonVO::Wrapper MedicalOrderExecutionService::execStartExecution(
    const ExecuteOrderDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    if (!dto->serviceRequestId || dto->serviceRequestId.getValue("").empty()) {
        result->fail(String("serviceRequestId is required"));
        return result;
    }
    if (!dto->performerId || dto->performerId.getValue("").empty()) {
        result->fail(String("performerId is required"));
        return result;
    }

    const std::string orderType = dto->orderType.getValue("SERVICE_REQUEST");
    if (orderType != "SERVICE_REQUEST" && orderType != "MED_REQUEST") {
        result->fail(String("Unsupported order type"));
        return result;
    }

    MedicalOrderExecutionDAO dao;
	const auto existingProcedureId=dao.selectActiveCompletedProcedureId(
		orderType,dto->serviceRequestId.getValue(""),dto->encounterId.getValue(""),
		dto->planTime.getValue(""));
	if(!existingProcedureId.empty()){
		result->success(String(existingProcedureId.c_str()));
		return result;
	}
    const std::string procedureId = UuidFacade().genUuid();
    dao.getSqlSession()->beginTransaction();
    try {
        if (dao.insertCompletedProcedure(
                procedureId, orderType,
				dto->serviceRequestId.getValue(""), dto->encounterId.getValue(""),
				dto->planTime.getValue(""), dto->performerId.getValue("")) != 1
            || dao.completeOrder(
                orderType,
				dto->serviceRequestId.getValue(""), dto->encounterId.getValue(""),
				dto->performerId.getValue("")) != 1) {
            throw std::runtime_error("Execution write failed.");
        }
        dao.getSqlSession()->commitTransaction();
        result->success(String(procedureId.c_str()));
    }
    catch (...) {
        dao.getSqlSession()->rollbackTransaction();
        result->fail(String("Execution failed: order not found or not in verified status"));
    }
    return result;
}

// ============================================================================
// Requirement 6: complete execution
// ============================================================================
StringJsonVO::Wrapper MedicalOrderExecutionService::execCompleteExecution(
    const CompleteOrderDTO::Wrapper& dto)
{
    auto result = StringJsonVO::createShared();

    if (!dto->serviceRequestId || dto->serviceRequestId.getValue("").empty()) {
        result->fail(String("serviceRequestId is required"));
        return result;
    }
    if (!dto->performerId || dto->performerId.getValue("").empty()) {
        result->fail(String("performerId is required"));
        return result;
    }

    MedicalOrderExecutionDAO dao;
    int rows = dao.completeExecution(
        dto->serviceRequestId.getValue(""),
        dto->performerId.getValue(""));

    if (rows > 0) {
        result->success(String("Execution completed"));
    } else {
        result->fail(String("Complete execution failed: order not found or not in executing status"));
    }
    return result;
}
