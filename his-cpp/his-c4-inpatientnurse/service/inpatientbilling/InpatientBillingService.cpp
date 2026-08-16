#include "stdafx.h"
#include "InpatientBillingService.h"
#include "dao/inpatientbilling/InpatientBillingDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

namespace
{
const std::string BILLING_RESULT_MARKER = "\n--RESULT--\n";

template<class DTO>
std::string billingFingerprint(const oatpp::Object<DTO>& dto)
{
	return oatpp::parser::json::mapping::ObjectMapper::createShared()
		->writeToString(dto).getValue("");
}

template<class ResponseDTO>
oatpp::Object<ResponseDTO> restoreBillingResult(
	InpatientBillingDAO& dao, const std::string& operation,
	const std::string& idempotencyKey, const std::string& fingerprint)
{
	const auto values = dao.selectIdempotencyResult(operation, idempotencyKey);
	if (values.empty()) return nullptr;
	const auto position = values.front().find(BILLING_RESULT_MARKER);
	if (position == std::string::npos
		|| values.front().substr(0, position) != fingerprint) return nullptr;
	return oatpp::parser::json::mapping::ObjectMapper::createShared()
		->readFromString<oatpp::Object<ResponseDTO>>(
			values.front().substr(position + BILLING_RESULT_MARKER.size()).c_str());
}

template<class ResponseDTO>
bool saveBillingResult(InpatientBillingDAO& dao, const std::string& operation,
	const std::string& idempotencyKey, const std::string& operatorId,
	const std::string& fingerprint, const oatpp::Object<ResponseDTO>& response)
{
	const auto resultData = oatpp::parser::json::mapping::ObjectMapper::createShared()
		->writeToString(response).getValue("");
	return dao.insertIdempotencyResult(UuidFacade().genUuid(), operation,
		idempotencyKey, operatorId, fingerprint, resultData) == 1;
}

bool validPricingItem(const ConfirmDirectPricingItemDTO::Wrapper& item)
{
	return item && item->encounterId && !item->encounterId->empty()
		&& item->patientId && !item->patientId->empty()
		&& item->accountId && !item->accountId->empty()
		&& item->productDefinitionId && !item->productDefinitionId->empty()
		&& item->quantity && item->quantity.getValue(0.0) > 0.0
		&& item->unitCode && !item->unitCode->empty();
}

ConfirmDirectPricingItemDTO::Wrapper toDirectPricingItem(
	const ConfirmSupplementPricingItemDTO::Wrapper& source)
{
	auto target = ConfirmDirectPricingItemDTO::createShared();
	target->encounterId = source->encounterId;
	target->patientId = source->patientId;
	target->enabled = true;
	target->accountId = source->accountId;
	target->productTableName = source->productTableName;
	target->productDefinitionId = source->productDefinitionId;
	target->quantity = source->quantity;
	target->unitCode = source->unitCode;
	target->chargeItemDefinitionId = source->chargeItemDefinitionId;
	target->unitPrice = source->unitPrice;
	target->totalPrice = source->totalPrice;
	target->remark = source->remark;
	target->performLocation = source->performLocation;
	return target;
}
}

PagePricingCatalogDTO::Wrapper InpatientBillingService::pagePricingCatalog(
	const PagePricingCatalogQuery::Wrapper& query)
{
	auto page = PagePricingCatalogDTO::createShared();
	page->rows = oatpp::List<NurseStationPricingCatalogRowDTO::Wrapper>::createShared();
	page->total = static_cast<int64_t>(0);
	page->pages = static_cast<int64_t>(0);
	if (!query || query->pageIndex.getValue(0) < 1 || query->pageSize.getValue(0) < 1) {
		return page;
	}
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	InpatientBillingDAO dao;
	page->total = dao.countPricingCatalog(query);
	page->calcPages();
	for (auto& row : dao.selectPricingCatalog(query)) {
		page->addData(row);
	}
	return page;
}

ConfirmDirectPricingResponseDTO::Wrapper InpatientBillingService::confirmDirectPricing(
	const ConfirmDirectPricingDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = ConfirmDirectPricingResponseDTO::createShared();
	response->success = false;
	response->processedCount = 0;
	response->createdServiceRequestIds = oatpp::List<oatpp::String>::createShared();
	response->createdDeviceRequestIds = oatpp::List<oatpp::String>::createShared();
	response->results = oatpp::List<PricingWriteResultDTO::Wrapper>::createShared();
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty()
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) {
		return response;
	}

	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<ConfirmDirectPricingResponseDTO>(
		dao, "DIRECT_PRICING", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"DIRECT_PRICING", dto->idempotencyKey.getValue("")).empty()) return response;
	UuidFacade uuid;
	const std::string now = SimpleDateTimeFormat::format();
	dao.getSqlSession()->beginTransaction();
	try {
		for (const auto& item : *dto->items) {
			auto result = PricingWriteResultDTO::createShared();
			result->clientRowId = item->clientRowId;
			if (!validPricingItem(item) || (item->enabled && !item->enabled.getValue(false))
				|| !dao.prepareNursePricingItem(item)) {
				throw std::runtime_error("Invalid pricing item.");
			}
			const std::string requestId = uuid.genUuid();
			const std::string chargeItemId = uuid.genUuid();
			const bool device = item->productTableName == "adm_device_definition";
			std::string dispenseId;
			bool created = false;
			if (device) {
				dispenseId = uuid.genUuid();
				created = dao.insertNursePricingDeviceRequest(
					requestId, item, "", operatorId, now) == 1
					&& dao.insertNursePricingDeviceDispense(
						dispenseId, requestId, item, "", operatorId, now) == 1;
			}
			else {
				created = dao.insertNursePricingServiceRequest(
					requestId, item, "", operatorId, now) == 1;
			}
			if (!created || dao.insertNursePricingChargeItem(
				chargeItemId, requestId, dispenseId, item, operatorId, now) != 1) {
				throw std::runtime_error("Pricing write failed.");
			}
			(device ? response->createdDeviceRequestIds
				: response->createdServiceRequestIds)->push_back(requestId);
			result->success = true;
			result->requestId = requestId.c_str();
			result->finalUnitPrice = item->unitPrice;
			result->finalTotalPrice = item->totalPrice;
			result->message = ZH_WORDS_GETTER("business-text.billing.pricing-success");
			response->results->push_back(result);
			response->processedCount = response->processedCount + 1;
		}
		response->success = true;
		if (!saveBillingResult(dao, "DIRECT_PRICING",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...) {
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
		response->processedCount = 0;
		response->createdServiceRequestIds = oatpp::List<oatpp::String>::createShared();
		response->createdDeviceRequestIds = oatpp::List<oatpp::String>::createShared();
		response->results = oatpp::List<PricingWriteResultDTO::Wrapper>::createShared();
	}
	return response;
}

ListPricedOrdersDTO::Wrapper InpatientBillingService::listPricedOrders(
	const ListPricedOrdersQuery::Wrapper& query)
{
	auto response = ListPricedOrdersDTO::createShared();
	response->groups = oatpp::List<PricedOrderGroupDTO::Wrapper>::createShared();
	if (!query || ((!query->encounterId || query->encounterId->empty())
		&& (!query->encounterIds || query->encounterIds->empty()))) {
		return response;
	}
	for (auto& group : InpatientBillingDAO().selectPricedOrders(query)) {
		response->groups->push_back(group);
	}
	return response;
}

ListSupplementParentOrdersDTO::Wrapper InpatientBillingService::listSupplementParentOrders(
	const ListSupplementParentOrdersQuery::Wrapper& query)
{
	auto response = ListSupplementParentOrdersDTO::createShared();
	response->groups = oatpp::List<SupplementParentOrderGroupDTO::Wrapper>::createShared();
	if (!query || ((!query->encounterId || query->encounterId->empty())
		&& (!query->encounterIds || query->encounterIds->empty()))) {
		return response;
	}
	for (auto& group : InpatientBillingDAO().selectSupplementParentOrders(query)) {
		response->groups->push_back(group);
	}
	return response;
}

ConfirmSupplementPricingResponseDTO::Wrapper InpatientBillingService::confirmSupplementPricing(
	const ConfirmSupplementPricingDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = ConfirmSupplementPricingResponseDTO::createShared();
	response->success = false;
	response->processedCount = 0;
	response->createdServiceRequestIds = oatpp::List<oatpp::String>::createShared();
	response->createdDeviceRequestIds = oatpp::List<oatpp::String>::createShared();
	response->results = oatpp::List<PricingWriteResultDTO::Wrapper>::createShared();
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty()
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) {
		return response;
	}

	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<ConfirmSupplementPricingResponseDTO>(
		dao, "SUPPLEMENT_PRICING", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"SUPPLEMENT_PRICING", dto->idempotencyKey.getValue("")).empty()) return response;
	UuidFacade uuid;
	const std::string now = SimpleDateTimeFormat::format();
	dao.getSqlSession()->beginTransaction();
	try {
		for (const auto& source : *dto->items) {
			if (!source || !source->parentRequestId || source->parentRequestId->empty()
				|| !source->parentOrderType || source->parentOrderType->empty()
				|| !dao.isAllowedSupplementParent(
					source->parentRequestId.getValue(""), source->encounterId.getValue(""),
					source->parentOrderType.getValue(""))) {
				throw std::runtime_error("Missing parent request anchor.");
			}
			auto result = PricingWriteResultDTO::createShared();
			result->clientRowId = source->clientRowId;
			auto item = toDirectPricingItem(source);
			if (!validPricingItem(item) || !dao.prepareNursePricingItem(item)) {
				throw std::runtime_error("Invalid supplement pricing item.");
			}
			const std::string requestId = uuid.genUuid();
			const std::string chargeItemId = uuid.genUuid();
			const bool device = item->productTableName == "adm_device_definition";
			std::string dispenseId;
			bool created = false;
			if (device) {
				dispenseId = uuid.genUuid();
				created = dao.insertNursePricingDeviceRequest(
					requestId, item, source->parentRequestId, operatorId, now) == 1
					&& dao.insertNursePricingDeviceDispense(dispenseId, requestId, item,
						source->parentRequestId, operatorId, now) == 1;
			}
			else {
				created = dao.insertNursePricingServiceRequest(
					requestId, item, source->parentRequestId, operatorId, now) == 1;
			}
			if (!created || dao.insertNursePricingChargeItem(
				chargeItemId, requestId, dispenseId, item, operatorId, now) != 1) {
				throw std::runtime_error("Supplement pricing write failed.");
			}
			(device ? response->createdDeviceRequestIds
				: response->createdServiceRequestIds)->push_back(requestId);
			result->success = true;
			result->requestId = requestId.c_str();
			result->finalUnitPrice = item->unitPrice;
			result->finalTotalPrice = item->totalPrice;
			result->message = ZH_WORDS_GETTER("business-text.billing.pricing-success");
			response->results->push_back(result);
			response->processedCount = response->processedCount + 1;
		}
		response->success = true;
		if (!saveBillingResult(dao, "SUPPLEMENT_PRICING",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...) {
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
		response->processedCount = 0;
		response->createdServiceRequestIds = oatpp::List<oatpp::String>::createShared();
		response->createdDeviceRequestIds = oatpp::List<oatpp::String>::createShared();
		response->results = oatpp::List<PricingWriteResultDTO::Wrapper>::createShared();
	}
	return response;
}

CancelPricingResponseDTO::Wrapper InpatientBillingService::cancelPricing(
	const CancelPricingDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = CancelPricingResponseDTO::createShared();
	response->success = false;
	response->processedCount = 0;
	response->cancelledServiceRequestIds = oatpp::List<oatpp::String>::createShared();
	response->cancelledDeviceRequestIds = oatpp::List<oatpp::String>::createShared();
	response->results = oatpp::List<PricingWriteResultDTO::Wrapper>::createShared();
	if (!dto || !dto->idempotencyKey || dto->idempotencyKey->empty()
		|| !dto->reason || dto->reason->empty()
		|| !dto->items || dto->items->empty() || dto->items->size() > 50) {
		return response;
	}

	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<CancelPricingResponseDTO>(
		dao, "CANCEL_PRICING", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"CANCEL_PRICING", dto->idempotencyKey.getValue("")).empty()) return response;
	const std::string now = SimpleDateTimeFormat::format();
	dao.getSqlSession()->beginTransaction();
	try {
		std::set<std::string> requestKeys;
		for (const auto& item : *dto->items) {
			auto result = PricingWriteResultDTO::createShared();
			result->clientRowId = item->requestId;
			if (!item || !item->requestId || item->requestId->empty()
				|| !item->encounterId || item->encounterId->empty()
				|| !item->orderType || item->orderType->empty()
				|| !requestKeys.insert(item->orderType.getValue("") + "\x01"
					+ item->requestId.getValue("")).second) {
				throw std::runtime_error("Invalid cancellation item.");
			}
			if (dao.softDeletePendingChargeItems(
				item->requestId, item->encounterId, operatorId, now) < 1
				|| dao.softDeleteNursePricingRequest(
					item->orderType, item->requestId, item->encounterId, operatorId, now) != 1) {
				throw std::runtime_error("Pricing cancellation failed.");
			}
			(item->orderType == "DEVICE_REQUEST" ? response->cancelledDeviceRequestIds
				: response->cancelledServiceRequestIds)->push_back(item->requestId);
			result->success = true;
			result->requestId = item->requestId;
			result->message = ZH_WORDS_GETTER("business-text.billing.cancel-success");
			response->results->push_back(result);
			response->processedCount = response->processedCount + 1;
		}
		response->success = true;
		if (!saveBillingResult(dao, "CANCEL_PRICING",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...) {
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
		response->processedCount = 0;
		response->cancelledServiceRequestIds = oatpp::List<oatpp::String>::createShared();
		response->cancelledDeviceRequestIds = oatpp::List<oatpp::String>::createShared();
		response->results = oatpp::List<PricingWriteResultDTO::Wrapper>::createShared();
	}
	return response;
}

ListAutoRollConfigsDTO::Wrapper InpatientBillingService::listAutoRollConfigs(const ListAutoRollConfigsQuery::Wrapper& query)
{
	auto result = ListAutoRollConfigsDTO::createShared();
	result->configs = oatpp::List<AutoRollConfigDTO::Wrapper>::createShared();
	result->nursingLevelReadonly = false;
	if (!query|| !query->encounterId || query->encounterId->empty()) return result;
	InpatientBillingDAO dao;
	auto rules = dao.selectRulesByEncounterId(query->encounterId.getValue(""));
	for (auto& rule : rules)
	{
		auto dto = AutoRollConfigDTO::createShared();
		dto->ruleId = rule.getId();
		dto->ruleName = rule.getRuleName();
		dto->activityId = rule.getTriggerSourceId();
		dto->activityName = "";
		dto->chargeDefinitionId = rule.getChargeDefinitionId();
		dto->quantityValue = rule.getQuantityValue();
		dto->isActive = (rule.getStatusEnum() == "ACTIVE");
		dto->version = rule.getVersion();
		dto->statusEnum = rule.getStatusEnum();
		dto->triggerSourceTable = rule.getTriggerSourceTable();
		dto->performingOrgId = rule.getPerformingOrgId();
		dto->priority = rule.getPriority();
		dto->remark = rule.getRemark();
		dto->updatedAt = rule.getUpdatedAt();
		result->configs->push_back(dto);
	}
	return result;
}

CreateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::createAutoGenRule(
	const CreateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = CreateAutoGenRuleResponseDTO::createShared();
	response->ruleId = "";
	if (!dto) return response;
	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<CreateAutoGenRuleResponseDTO>(
		dao, "AUTO_RULE_CREATE", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"AUTO_RULE_CREATE", dto->idempotencyKey.getValue("")).empty()) return response;
	if (!dao.isValidEncounterPatient(
			dto->encounterId.getValue(""), dto->patientId.getValue(""))
		|| !dao.isValidAutoRuleReferences(
			dto->activityId.getValue(""), dto->chargeDefinitionId.getValue("")))
		return response;
	AutoGenRuleDO data;
	UuidFacade uf;
	data.setId(uf.genUuid());
	data.setEncounterId(dto->encounterId.getValue(""));
	data.setRuleName(dto->ruleName.getValue(""));
	data.setTriggerSourceTable("wkf_activity_definition");
	data.setTriggerSourceId(dto->activityId.getValue(""));
	data.setChargeDefinitionId(dto->chargeDefinitionId.getValue(""));
	data.setQuantityValue(dto->quantityValue ? dto->quantityValue.getValue(1.0) : 1.0);
	data.setPerformingOrgId(dto->performingOrgId ? dto->performingOrgId.getValue("") : "");
	data.setRemark(dto->remark ? dto->remark.getValue("") : "");
	int priority = 0;
	if (dto->priority && !dto->priority->empty())
	{
		try { priority = std::stoi(dto->priority.getValue("0")); }
		catch (...) {}
	}
	data.setPriority(priority);
	data.setStatusEnum("ACTIVE");
	data.setIsDeleted(false);
	data.setVersion(0);
	data.setCreatedAt(SimpleDateTimeFormat::format());
	data.setCreatedBy(operatorId);
	dao.getSqlSession()->beginTransaction();
	try
	{
		if (dao.insert(data) != 1)
			throw std::runtime_error("Auto billing rule create failed.");
		response->ruleId = data.getId();
		response->version = static_cast<v_int64>(0);
		if (!saveBillingResult(dao, "AUTO_RULE_CREATE",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		response->ruleId = "";
	}
	return response;
}

UpdateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::updateAutoGenRule(
	const UpdateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = UpdateAutoGenRuleResponseDTO::createShared();
	response->success = false;
	if (!dto) return response;
	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<UpdateAutoGenRuleResponseDTO>(
		dao, "AUTO_RULE_UPDATE", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"AUTO_RULE_UPDATE", dto->idempotencyKey.getValue("")).empty()) return response;
	if (!dao.isValidEncounterPatient(
			dto->encounterId.getValue(""), dto->patientId.getValue("")))
		return response;
	if (dto->activityId && !dto->activityId->empty()
		&& !dao.isActiveActivity(dto->activityId.getValue("")))
		return response;
	if (dto->chargeDefinitionId && !dto->chargeDefinitionId->empty()
		&& !dao.isActiveChargeDefinition(dto->chargeDefinitionId.getValue("")))
		return response;
	dao.getSqlSession()->beginTransaction();
	try
	{
		response->success = dao.updateAutoGenRule(dto) == 1;
		if (!response->success) throw std::runtime_error("Auto billing rule update conflict.");
		response->version = dto->expectedVersion.getValue(0) + 1;
		if (!saveBillingResult(dao, "AUTO_RULE_UPDATE",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
	}
	return response;
}

DeleteAutoGenRuleResponseDTO::Wrapper InpatientBillingService::deleteAutoGenRule(
	const DeleteAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = DeleteAutoGenRuleResponseDTO::createShared();
	response->success = false;
	if (!dto) return response;
	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<DeleteAutoGenRuleResponseDTO>(
		dao, "AUTO_RULE_DELETE", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"AUTO_RULE_DELETE", dto->idempotencyKey.getValue("")).empty()) return response;
	const bool valid = dto->ruleId && dto->encounterId
		&& dao.isValidEncounterPatient(
			dto->encounterId.getValue(""), dto->patientId.getValue(""));
	if (!valid) return response;
	dao.getSqlSession()->beginTransaction();
	try
	{
		response->success = dao.softDeleteAutoGenRule(
			dto->ruleId.getValue(""), dto->encounterId.getValue(""),
			dto->expectedVersion.getValue(-1)) == 1;
		if (!response->success) throw std::runtime_error("Auto billing rule delete conflict.");
		if (!saveBillingResult(dao, "AUTO_RULE_DELETE",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
	}
	return response;
}

ActivateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::activateAutoGenRule(
	const ActivateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = ActivateAutoGenRuleResponseDTO::createShared();
	response->success = false;
	if (!dto) return response;
	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<ActivateAutoGenRuleResponseDTO>(
		dao, "AUTO_RULE_STATUS", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"AUTO_RULE_STATUS", dto->idempotencyKey.getValue("")).empty()) return response;
	const bool valid = dto->ruleId && dto->encounterId
		&& dao.isValidEncounterPatient(
			dto->encounterId.getValue(""), dto->patientId.getValue(""));
	if (!valid) return response;
	dao.getSqlSession()->beginTransaction();
	try
	{
		response->success = dao.setAutoGenRuleStatus(
			dto->ruleId.getValue(""), dto->encounterId.getValue(""), "ACTIVE",
			dto->expectedVersion.getValue(-1)) == 1;
		if (!response->success) throw std::runtime_error("Auto billing rule status conflict.");
		response->version = dto->expectedVersion.getValue(0) + 1;
		response->statusEnum = "ACTIVE";
		if (!saveBillingResult(dao, "AUTO_RULE_STATUS",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
	}
	return response;
}

ActivateAutoGenRuleResponseDTO::Wrapper InpatientBillingService::suspendAutoGenRule(
	const ActivateAutoGenRuleDTO::Wrapper& dto, const std::string& operatorId)
{
	auto response = ActivateAutoGenRuleResponseDTO::createShared();
	response->success = false;
	if (!dto) return response;
	InpatientBillingDAO dao;
	const auto fingerprint = billingFingerprint(dto);
	const auto restored = restoreBillingResult<ActivateAutoGenRuleResponseDTO>(
		dao, "AUTO_RULE_STATUS", dto->idempotencyKey.getValue(""), fingerprint);
	if (restored) return restored;
	if (!dao.selectIdempotencyResult(
		"AUTO_RULE_STATUS", dto->idempotencyKey.getValue("")).empty()) return response;
	const bool valid = dto->ruleId && dto->encounterId
		&& dao.isValidEncounterPatient(
			dto->encounterId.getValue(""), dto->patientId.getValue(""));
	if (!valid) return response;
	dao.getSqlSession()->beginTransaction();
	try
	{
		response->success = dao.setAutoGenRuleStatus(
			dto->ruleId.getValue(""), dto->encounterId.getValue(""), "INACTIVE",
			dto->expectedVersion.getValue(-1)) == 1;
		if (!response->success) throw std::runtime_error("Auto billing rule status conflict.");
		response->version = dto->expectedVersion.getValue(0) + 1;
		response->statusEnum = "INACTIVE";
		if (!saveBillingResult(dao, "AUTO_RULE_STATUS",
			dto->idempotencyKey.getValue(""), operatorId, fingerprint, response))
			throw std::runtime_error("Billing idempotency write failed.");
		dao.getSqlSession()->commitTransaction();
	}
	catch (...)
	{
		dao.getSqlSession()->rollbackTransaction();
		response->success = false;
	}
	return response;
}
