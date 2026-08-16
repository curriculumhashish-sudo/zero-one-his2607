#include "stdafx.h"
#include "GetSettleHistoryService.h"

#include <list>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <vector>

#include "dao/settle/GetSettleHistoryDAO.h"
#include "service/settle/SettlementValidator.h"

namespace {
	double parseHistoryAmount(const std::string& value) {
		return value.empty() ? 0 : std::stod(value);
	}
}

GetSettleHistoryPageDTO::Wrapper
GetSettleHistoryService::getSettleHistory(
	const GetSettleHistoryQuery::Wrapper& query) {
	const std::string validationMessage =
		SettlementValidator::validateSettleHistoryQuery(query);
	if (!validationMessage.empty()) {
		throw std::invalid_argument(validationMessage);
	}

	const std::string presentationMode =
		query->presentationMode.getValue("DETAIL");

	auto page = GetSettleHistoryPageDTO::createShared();
	page->pageIndex = query->pageIndex;
	page->pageSize = query->pageSize;
	page->rows->clear();

	GetSettleHistoryDAO dao;
	const uint64_t total = dao.count(query);
	page->total = static_cast<int64_t>(total);
	page->calcPages();
	if (total == 0) {
		return page;
	}

	const std::list<PtrGetSettleHistoryDO> records =
		dao.selectWithPage(query);
	std::vector<std::string> settlementIds;
	std::unordered_map<std::string, SettleHistoryItemDTO::Wrapper>
		recordDtoMap;
	settlementIds.reserve(records.size());
	recordDtoMap.reserve(records.size());

	for (const auto& record : records) {
		auto itemDto = SettleHistoryItemDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO_1(
			itemDto, record,
			settlementId, SettlementId,
			paymentNo, PaymentNo,
			patientId, PatientId,
			patientName, PatientName,
			encounterId, EncounterId,
			encounterBusNo, EncounterBusNo,
			kindEnum, KindEnum,
			statusEnum, StatusEnum,
			billDate, BillDate,
			invoiceId, InvoiceId,
			setlId, SetlId);
		itemDto->displayAmount = parseHistoryAmount(record->getDisplayAmount());
		itemDto->tenderedAmount = parseHistoryAmount(record->getTenderedAmount());
		itemDto->returnedAmount = parseHistoryAmount(record->getReturnedAmount());
		itemDto->categories->clear();
		itemDto->lines->clear();
		itemDto->payments->clear();

		// 补充前端联调所需的聚合字段
		itemDto->activePaymentId = record->getSettlementId();
		itemDto->receivableTotal = itemDto->displayAmount;
		itemDto->netSettleTotal =
			itemDto->tenderedAmount.getValue(0) - itemDto->returnedAmount.getValue(0);
		itemDto->settleTime = record->getBillDate();

		const std::string settlementId = record->getSettlementId();
		settlementIds.push_back(settlementId);
		recordDtoMap.emplace(settlementId, itemDto);
		page->rows->push_back(itemDto);
	}

	// SUMMARY模式仅返回汇总字段
	if (presentationMode == "SUMMARY") {
		return page;
	}

	// 查询本页所有结算记录的支付明细
	const std::list<PtrSettleHistoryPaymentDetailDO> paymentDetails =
		dao.selectPaymentDetails(settlementIds);
	for (const auto& paymentDetail : paymentDetails) {
		const auto recordIterator =
			recordDtoMap.find(paymentDetail->getReconciliationId());
		if (recordIterator == recordDtoMap.end()) {
			continue;
		}

		auto detailDto = SettlePaymentDetailDTO::createShared();
		ZO_STAR_DOMAIN_DO_TO_DTO_1(
			detailDto, paymentDetail,
			paymentDetailId, PaymentDetailId,
			payEnum, PayEnum,
			payTransNo, PayTransNo,
			payTransDate, PayTransDate,
			resultEnum, ResultEnum);
		detailDto->amount = parseHistoryAmount(paymentDetail->getAmount());
		detailDto->returnAmount =
			parseHistoryAmount(paymentDetail->getReturnAmount());
		recordIterator->second->payments->push_back(detailDto);
	}

	// 收集本页结算记录涉及的收费明细主键
	std::unordered_map<std::string, std::vector<std::string>> recordItemIds;
	std::set<std::string> allItemIds;
	for (const auto& record : records) {
		const std::string idsText = record->getChargeItemIds();
		if (idsText.empty()) {
			continue;
		}
		std::istringstream stream(idsText);
		std::string itemId;
		while (std::getline(stream, itemId, ',')) {
			if (itemId.empty()) {
				continue;
			}
			recordItemIds[record->getSettlementId()].push_back(itemId);
			allItemIds.insert(itemId);
		}
	}

	// 批量查询收费明细，按结算记录组装明细行和分类汇总
	if (!allItemIds.empty()) {
		std::vector<std::string> itemIds(allItemIds.begin(), allItemIds.end());
		const std::list<PtrCostDetailItemDO> chargeItems =
			dao.selectChargeItemsByIds(itemIds);
		std::unordered_map<std::string, PtrCostDetailItemDO> itemMap;
		for (const auto& item : chargeItems) {
			itemMap.emplace(item->getChargeItemId(), item);
		}

		for (const auto& record : records) {
			const auto recordIterator = recordDtoMap.find(record->getSettlementId());
			if (recordIterator == recordDtoMap.end()) {
				continue;
			}
			const auto& itemDto = recordIterator->second;

			const auto idIterator = recordItemIds.find(record->getSettlementId());
			if (idIterator == recordItemIds.end()) {
				continue;
			}

			// 按费用类别汇总
			std::map<std::string, SettleHistoryCategoryDTO::Wrapper> categoryMap;
			for (const auto& itemId : idIterator->second) {
				const auto itemIterator = itemMap.find(itemId);
				if (itemIterator == itemMap.end()) {
					continue;
				}
				const auto& item = itemIterator->second;

				auto lineDto = SettleHistoryLineDTO::createShared();
				ZO_STAR_DOMAIN_DO_TO_DTO_1(
					lineDto, item,
					chargeItemId, ChargeItemId,
					itemName, ItemName,
					chargeCategoryCode, ChargeCategoryCode,
					chargeCategoryName, ChargeCategoryName,
					quantityUnit, QuantityUnit,
					statusEnum, StatusEnum,
					occurrenceTime, OccurrenceTime);
				lineDto->quantityValue = parseHistoryAmount(item->getQuantityValue());
				lineDto->unitPrice = parseHistoryAmount(item->getUnitPrice());
				lineDto->totalPrice = parseHistoryAmount(item->getTotalPrice());
				itemDto->lines->push_back(lineDto);

				const std::string categoryCode = item->getChargeCategoryCode();
				const auto categoryIterator = categoryMap.find(categoryCode);
				if (categoryIterator == categoryMap.end()) {
					auto categoryDto = SettleHistoryCategoryDTO::createShared();
					categoryDto->categoryCode = categoryCode;
					categoryDto->categoryName = item->getChargeCategoryName();
					categoryDto->lineCount = 1;
					categoryDto->totalAmount = lineDto->totalPrice;
					categoryMap.emplace(categoryCode, categoryDto);
				}
				else {
					categoryIterator->second->lineCount =
						categoryIterator->second->lineCount.getValue(0) + 1;
					categoryIterator->second->totalAmount =
						categoryIterator->second->totalAmount.getValue(0) +
						lineDto->totalPrice.getValue(0);
				}
			}
			for (const auto& categoryPair : categoryMap) {
				itemDto->categories->push_back(categoryPair.second);
			}
		}
	}
	return page;
}
