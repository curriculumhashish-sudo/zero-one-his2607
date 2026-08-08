#include "stdafx.h"
#include "GetCostDetailService.h"

#include <list>
#include <stdexcept>
#include <string>

#include "dao/settle/GetCostDetailDAO.h"
#include "service/settle/SettlementValidator.h"

namespace {
	double parseAmount(const std::string &value) {
		return value.empty() ? 0 : std::stod(value);
	}
}

GetCostDetailDTO::Wrapper GetCostDetailService::getCostDetail(
	const GetCostDetailQuery::Wrapper &query) {
	const std::string validationMessage =
		SettlementValidator::validateCostDetailQuery(query);
	if (!validationMessage.empty()) {
		throw std::invalid_argument(validationMessage);
	}

	GetCostDetailDAO dao;
	const PtrGetCostDetailDO summary = dao.selectSummary(query);
	if (!summary) {
		throw std::invalid_argument(ZH_WORDS_GETTER("ovo.charging.settle.encounter_not_found"));
	}

	auto result = GetCostDetailDTO::createShared();
	ZO_STAR_DOMAIN_DO_TO_DTO_1(
		result, summary,
		patientId, PatientId,
		patientName, PatientName,
		encounterId, EncounterId,
		encounterBusNo, EncounterBusNo);
	result->totalAmount = parseAmount(summary->getTotalAmount());
	result->settledAmount = parseAmount(summary->getSettledAmount());
	result->unsettledAmount = parseAmount(summary->getUnsettledAmount());
	result->items->clear();

	// SUMMARY模式仅返回汇总，不查询费用明细
	const std::string presentationMode = query->presentationMode.getValue("DETAIL");
	if (presentationMode == "SUMMARY") {
		return result;
	}

	const std::list<PtrCostDetailItemDO> items = dao.selectItems(query);
	for (const auto &item : items) {
		auto itemDto = CostDetailItemDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO_1(
			itemDto, item,
			chargeItemId, ChargeItemId,
			busNo, BusNo,
			prescriptionNo, PrescriptionNo,
			itemName, ItemName,
			chargeCategoryCode, ChargeCategoryCode,
			chargeCategoryName, ChargeCategoryName,
			quantityUnit, QuantityUnit,
			statusEnum, StatusEnum,
			occurrenceTime, OccurrenceTime,
			performingOrgName, PerformingOrgName,
			entererName, EntererName);
		itemDto->quantityValue = parseAmount(item->getQuantityValue());
		itemDto->unitPrice = parseAmount(item->getUnitPrice());
		itemDto->totalPrice = parseAmount(item->getTotalPrice());
		result->items->push_back(itemDto);
	}
	return result;
}
