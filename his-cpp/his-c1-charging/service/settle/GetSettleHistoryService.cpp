#include "stdafx.h"
#include "GetSettleHistoryService.h"

#include <list>
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
		itemDto->paymentDetails->clear();

		const std::string settlementId = record->getSettlementId();
		settlementIds.push_back(settlementId);
		recordDtoMap.emplace(settlementId, itemDto);
		page->rows->push_back(itemDto);
	}

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
		recordIterator->second->paymentDetails->push_back(detailDto);
	}
	return page;
}
