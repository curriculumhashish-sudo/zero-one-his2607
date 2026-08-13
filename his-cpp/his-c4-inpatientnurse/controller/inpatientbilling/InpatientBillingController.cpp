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
#include "stdafx.h"
#include "InpatientBillingController.h"
#include "service/inpatientbilling/InpatientBillingService.h"
#include "dao/common/NurseStationAccessDAO.h"

namespace
{
bool validPriority(const oatpp::String& priority)
{
    if (!inpatientnurse::validation::present(priority)) return true;
    const auto value = inpatientnurse::validation::trim(priority->c_str());
    return value == "NORMAL" || value == "URGENT"
		|| (value.size() <= 3 && inpatientnurse::validation::allDigits(value));
}

void normalizePriority(oatpp::String& priority)
{
    if (priority == "NORMAL") priority = "0";
    else if (priority == "URGENT") priority = "1";
}

bool validDirectPricing(const ConfirmDirectPricingDTO::Wrapper& dto)
{
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) return false;
	std::set<std::string> unique;
    for (const auto& item : *dto->items)
    {
        if (!item || !item->encounterId || item->encounterId->empty()
            || !item->patientId || item->patientId->empty()
            || !item->accountId || item->accountId->empty()
            || !item->productDefinitionId || item->productDefinitionId->empty()
            || !item->quantity.getPtr() || item->quantity.getValue(0.0) <= 0.0
            || !item->unitCode || item->unitCode->empty()
			|| !unique.insert(item->encounterId.getValue("") + ":"
				+ item->productDefinitionId.getValue("")).second)
            return false;
    }
    return true;
}

bool validSupplementPricing(const ConfirmSupplementPricingDTO::Wrapper& dto)
{
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) return false;
	std::set<std::string> unique;
    for (const auto& item : *dto->items)
    {
        if (!item || !item->encounterId || item->encounterId->empty()
            || !item->patientId || item->patientId->empty()
            || !item->accountId || item->accountId->empty()
            || !item->productDefinitionId || item->productDefinitionId->empty()
            || !item->quantity.getPtr() || item->quantity.getValue(0.0) <= 0.0
            || !item->unitCode || item->unitCode->empty()
			|| !inpatientnurse::validation::present(item->parentRequestId)
			|| !inpatientnurse::validation::oneOf(
				item->parentOrderType, {"MED_REQUEST", "SERVICE_REQUEST"}, false)
			|| !unique.insert(item->encounterId.getValue("") + ":"
				+ item->parentOrderType.getValue("") + ":"
				+ item->parentRequestId.getValue("") + ":"
				+ item->productDefinitionId.getValue("")).second)
            return false;
    }
    return true;
}

bool validCancelPricing(const CancelPricingDTO::Wrapper& dto)
{
	if (!dto || !inpatientnurse::validation::present(dto->idempotencyKey)
		|| dto->idempotencyKey->size() > 64
		|| !inpatientnurse::validation::present(dto->reason) || dto->reason->size() > 500
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) return false;
	std::set<std::string> unique;
    for (const auto& item : *dto->items)
    {
		if (!item || !inpatientnurse::validation::oneOf(
				item->orderType, {"DIRECT", "DEVICE_REQUEST", "SERVICE_REQUEST"}, false)
            || !item->requestId || item->requestId->empty()
			|| !item->encounterId || item->encounterId->empty()
			|| !unique.insert(item->orderType.getValue("") + ":"
				+ item->requestId.getValue("")).second)
            return false;
    }
    return true;
}

bool canAccessEncounters(const std::string& userId, const oatpp::String& encounterId,
	const oatpp::String& encounterIds)
{
	NurseStationAccessDAO dao;
	if (inpatientnurse::validation::present(encounterId)) {
		return dao.canAccessEncounter(userId, encounterId.getValue(""));
	}
	if (!inpatientnurse::validation::validCsv(encounterIds, 50, true)) return false;
	std::string values = encounterIds.getValue("");
	std::size_t begin = 0;
	while (begin <= values.size()) {
		const auto end = values.find(',', begin);
		const auto item = inpatientnurse::validation::trim(values.substr(begin,
			end == std::string::npos ? std::string::npos : end - begin));
		if (!dao.canAccessEncounter(userId, item)) return false;
		if (end == std::string::npos) break;
		begin = end + 1;
	}
	return true;
}

bool canAccessPatient(const PayloadDTO& payload, const oatpp::String& encounterId,
	const oatpp::String& patientId)
{
	return NurseStationAccessDAO().canAccessEncounterPatient(
		payload.getId(), encounterId.getValue(""), patientId.getValue(""));
}
}

PagePricingCatalogJsonVO::Wrapper InpatientBillingController::execPagePricingCatalog(
    const PagePricingCatalogQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = PagePricingCatalogJsonVO::createShared();
	if (!query || !inpatientnurse::validation::validPage(query->pageIndex, query->pageSize))
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	const bool hasEncounter = inpatientnurse::validation::present(query->encounterId);
	const bool hasPatient = inpatientnurse::validation::present(query->patientId);
	if (!hasEncounter || !hasPatient) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (hasEncounter && !canAccessPatient(payload, query->encounterId, query->patientId)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
    jvo->success(InpatientBillingService().pagePricingCatalog(query));
    return jvo;
}

ConfirmDirectPricingJsonVO::Wrapper InpatientBillingController::execConfirmDirectPricing(
    const ConfirmDirectPricingDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = ConfirmDirectPricingJsonVO::createShared();
    if (!validDirectPricing(dto))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
	for (const auto& item : *dto->items) {
		if (!canAccessPatient(payload, item->encounterId, item->patientId)) {
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
    auto result = InpatientBillingService().confirmDirectPricing(dto, payload.getId());
    result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

ListPricedOrdersJsonVO::Wrapper InpatientBillingController::execListPricedOrders(
    const ListPricedOrdersQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = ListPricedOrdersJsonVO::createShared();
	if (!query || (!inpatientnurse::validation::present(query->encounterId)
		&& !inpatientnurse::validation::validCsv(query->encounterIds, 50, true))
		|| !inpatientnurse::validation::validDateRange(
			query->pricedTimeFrom, query->pricedTimeTo))
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessEncounters(payload.getId(), query->encounterId, query->encounterIds)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
    jvo->success(InpatientBillingService().listPricedOrders(query));
    return jvo;
}

ListSupplementParentOrdersJsonVO::Wrapper InpatientBillingController::execListSupplementParentOrders(
    const ListSupplementParentOrdersQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = ListSupplementParentOrdersJsonVO::createShared();
	if (!query || (!inpatientnurse::validation::present(query->encounterId)
		&& !inpatientnurse::validation::validCsv(query->encounterIds, 50, true)))
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessEncounters(payload.getId(), query->encounterId, query->encounterIds)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
    jvo->success(InpatientBillingService().listSupplementParentOrders(query));
    return jvo;
}

ConfirmSupplementPricingJsonVO::Wrapper InpatientBillingController::execConfirmSupplementPricing(
    const ConfirmSupplementPricingDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = ConfirmSupplementPricingJsonVO::createShared();
    if (!validSupplementPricing(dto))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
	for (const auto& item : *dto->items) {
		if (!canAccessPatient(payload, item->encounterId, item->patientId)) {
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
    auto result = InpatientBillingService().confirmSupplementPricing(dto, payload.getId());
    result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

CancelPricingJsonVO::Wrapper InpatientBillingController::execCancelPricing(
    const CancelPricingDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = CancelPricingJsonVO::createShared();
    if (!validCancelPricing(dto))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
        return jvo;
    }
	for (const auto& item : *dto->items) {
		if (!NurseStationAccessDAO().canAccessEncounter(
			payload.getId(), item->encounterId.getValue(""))) {
			jvo->init(nullptr, RS_UNAUTHORIZED);
			return jvo;
		}
	}
    auto result = InpatientBillingService().cancelPricing(dto, payload.getId());
    result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

ListAutoRollConfigsJsonVO::Wrapper InpatientBillingController::execListAutoRollConfigs(
    const ListAutoRollConfigsQuery::Wrapper& query, const PayloadDTO& payload)
{
    auto jvo = ListAutoRollConfigsJsonVO::createShared();
	if (!query || !inpatientnurse::validation::present(query->encounterId)
		|| !inpatientnurse::validation::present(query->patientId)
		|| !inpatientnurse::validation::validDate(query->billDate))
	{
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessPatient(payload, query->encounterId, query->patientId)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
	auto result = InpatientBillingService().listAutoRollConfigs(query);
    jvo->success(result);
    return jvo;
}

CreateAutoGenRuleJsonVO::Wrapper InpatientBillingController::execCreateAutoGenRule(
    const CreateAutoGenRuleDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = CreateAutoGenRuleJsonVO::createShared();
	if (!dto || !dto->encounterId || dto->encounterId->empty()
		|| !inpatientnurse::validation::present(dto->idempotencyKey)
			|| dto->idempotencyKey->size() > 64
        || !dto->patientId || dto->patientId->empty()
        || !dto->ruleName || dto->ruleName->empty()
        || !dto->activityId || dto->activityId->empty()
		|| !dto->chargeDefinitionId || dto->chargeDefinitionId->empty()
		|| (dto->quantityValue.getPtr()
			&& (dto->quantityValue.getValue(0.0) <= 0.0
				|| dto->quantityValue.getValue(0.0) > 1000000.0))
		|| !validPriority(dto->priority))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessPatient(payload, dto->encounterId, dto->patientId)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
	normalizePriority(dto->priority);
	    auto result = InpatientBillingService().createAutoGenRule(dto, payload.getId());
    result->ruleId && !result->ruleId->empty() ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

UpdateAutoGenRuleJsonVO::Wrapper InpatientBillingController::execUpdateAutoGenRule(
    const UpdateAutoGenRuleDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = UpdateAutoGenRuleJsonVO::createShared();
	if (!dto || !dto->ruleId || dto->ruleId->empty()
		|| !inpatientnurse::validation::present(dto->idempotencyKey)
			|| dto->idempotencyKey->size() > 64
		|| !dto->expectedVersion.getPtr() || dto->expectedVersion.getValue(-1) < 0
        || !dto->encounterId || dto->encounterId->empty()
		|| !dto->patientId || dto->patientId->empty()
		|| (dto->quantityValue.getPtr()
			&& (dto->quantityValue.getValue(0.0) <= 0.0
				|| dto->quantityValue.getValue(0.0) > 1000000.0))
		|| !validPriority(dto->priority))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessPatient(payload, dto->encounterId, dto->patientId)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
	normalizePriority(dto->priority);
	    auto result = InpatientBillingService().updateAutoGenRule(dto, payload.getId());
    result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

DeleteAutoGenRuleJsonVO::Wrapper InpatientBillingController::execDeleteAutoGenRule(
    const DeleteAutoGenRuleDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = DeleteAutoGenRuleJsonVO::createShared();
	if (!dto || !dto->ruleId || dto->ruleId->empty()
		|| !inpatientnurse::validation::present(dto->idempotencyKey)
			|| dto->idempotencyKey->size() > 64
		|| !dto->expectedVersion.getPtr() || dto->expectedVersion.getValue(-1) < 0
        || !dto->encounterId || dto->encounterId->empty()
        || !dto->patientId || dto->patientId->empty())
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessPatient(payload, dto->encounterId, dto->patientId)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
	    auto result = InpatientBillingService().deleteAutoGenRule(dto, payload.getId());
    result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}

ActivateAutoGenRuleJsonVO::Wrapper InpatientBillingController::execActivateAutoGenRule(
    const ActivateAutoGenRuleDTO::Wrapper& dto, const PayloadDTO& payload)
{
    auto jvo = ActivateAutoGenRuleJsonVO::createShared();
	if (!dto || !dto->ruleId || dto->ruleId->empty()
		|| !inpatientnurse::validation::present(dto->idempotencyKey)
			|| dto->idempotencyKey->size() > 64
		|| !dto->expectedVersion.getPtr() || dto->expectedVersion.getValue(-1) < 0
        || !dto->encounterId || dto->encounterId->empty()
        || !dto->patientId || dto->patientId->empty()
		|| !inpatientnurse::validation::oneOf(
			dto->targetStatus, {"ACTIVE", "INACTIVE"}, false))
    {
        jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}
	if (!canAccessPatient(payload, dto->encounterId, dto->patientId)) {
		jvo->init(nullptr, RS_UNAUTHORIZED);
		return jvo;
	}
    auto result = dto->targetStatus == "INACTIVE"
	        ? InpatientBillingService().suspendAutoGenRule(dto, payload.getId())
	        : InpatientBillingService().activateAutoGenRule(dto, payload.getId());
    result->success ? jvo->success(result) : jvo->fail(result);
    return jvo;
}
