#include "stdafx.h"
#include "SettlementValidator.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <set>
#include <string>

namespace {
	constexpr uint64_t MAX_PAGE_SIZE = 100;
	constexpr uint64_t MAX_PAGE_INDEX = 1000000;
	constexpr size_t MAX_SETTLE_ITEM_COUNT = 500;
	constexpr double MONEY_EPSILON = 0.000001;
	constexpr double MAX_MONEY_AMOUNT = 1000000000000.0;

	bool isBlank(const std::string &value) {
		return value.empty() ||
			   std::all_of(value.begin(), value.end(), [](unsigned char character) { return std::isspace(character) != 0; });
	}

	bool isAllowedValue(const std::string &value, const std::set<std::string> &allowedValues) {
		return allowedValues.find(value) != allowedValues.end();
	}

	bool isLeapYear(int year) {
		return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
	}

	bool isValidDateTime(const std::string &value) {
		if (value.size() != 19) {
			return false;
		}

		const size_t separatorPositions[] = {4, 7, 10, 13, 16};
		const char separators[] = {'-', '-', ' ', ':', ':'};
		for (size_t index = 0; index < value.size(); ++index) {
			bool isSeparator = false;
			for (size_t separatorIndex = 0; separatorIndex < 5; ++separatorIndex) {
				if (index == separatorPositions[separatorIndex]) {
					if (value[index] != separators[separatorIndex]) {
						return false;
					}
					isSeparator = true;
					break;
				}
			}
			if (!isSeparator &&
				std::isdigit(static_cast<unsigned char>(value[index])) == 0) {
				return false;
			}
		}

		const int year = std::stoi(value.substr(0, 4));
		const int month = std::stoi(value.substr(5, 2));
		const int day = std::stoi(value.substr(8, 2));
		const int hour = std::stoi(value.substr(11, 2));
		const int minute = std::stoi(value.substr(14, 2));
		const int second = std::stoi(value.substr(17, 2));
		if (year < 2000 || month < 1 || month > 12 ||
			hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
			second < 0 || second > 59) {
			return false;
		}

		const int daysInMonth[] = {
			31, isLeapYear(year) ? 29 : 28, 31,
			30, 31, 30, 31, 31, 30, 31, 30, 31};
		return day >= 1 && day <= daysInMonth[month - 1];
	}
}

std::string SettlementValidator::validateCostDetailQuery(
	const GetCostDetailQuery::Wrapper &query) {
	if (!query) {
		return ZH_WORDS_GETTER("ovo.charging.settle.query_empty");
	}

	const std::string encounterId = query->encounterId.getValue("");
	const std::string patientId = query->patientId.getValue("");
	if (isBlank(encounterId) || encounterId.size() > 64) {
		return ZH_WORDS_GETTER("ovo.charging.settle.encounter_id_invalid");
	}
	if (isBlank(patientId) || patientId.size() > 64) {
		return ZH_WORDS_GETTER("ovo.charging.settle.patient_id_invalid");
	}

	const std::string statusEnum = query->statusEnum.getValue("ALL");
	const std::set<std::string> allowedStatuses = {
		"ALL", "DRAFT", "UNSETTLED", "SETTLED", "REFUNDED"};
	if (!isAllowedValue(statusEnum, allowedStatuses)) {
		return ZH_WORDS_GETTER("ovo.charging.settle.cost_status_invalid");
	}
	return "";
}

std::string SettlementValidator::validateSettleHistoryQuery(
	const GetSettleHistoryQuery::Wrapper &query) {
	if (!query) {
		return ZH_WORDS_GETTER("ovo.charging.settle.query_empty");
	}

	const std::string patientId = query->patientId.getValue("");
	const std::string encounterId = query->encounterId.getValue("");
	if (isBlank(patientId) || patientId.size() > 64) {
		return ZH_WORDS_GETTER("ovo.charging.settle.patient_id_invalid");
	}
	if (!encounterId.empty()) {
		if (isBlank(encounterId)) {
			return ZH_WORDS_GETTER("ovo.charging.settle.encounter_id_invalid");
		}
		if (encounterId.size() > 64) {
			return ZH_WORDS_GETTER("ovo.charging.settle.encounter_id_too_long");
		}
	}

	const uint64_t pageIndex = query->pageIndex.getValue(0);
	const uint64_t pageSize = query->pageSize.getValue(0);
	if (pageIndex == 0 || pageIndex > MAX_PAGE_INDEX) {
		return ZH_WORDS_GETTER("ovo.charging.settle.page_index_invalid");
	}
	if (pageSize == 0 || pageSize > MAX_PAGE_SIZE) {
		return ZH_WORDS_GETTER("ovo.charging.settle.page_size_invalid");
	}

	const std::string statusEnum = query->statusEnum.getValue("ALL");
	const std::set<std::string> allowedStatuses = {
		"ALL", "CONFIRMED", "SUCCESS", "REFUNDED", "CANCELLED"};
	if (!isAllowedValue(statusEnum, allowedStatuses)) {
		return ZH_WORDS_GETTER("ovo.charging.settle.history_status_invalid");
	}
	return "";
}

std::string SettlementValidator::validateSettleRequest(
	const SettleRequestDTO::Wrapper &dto) {
	if (!dto) {
		return ZH_WORDS_GETTER("ovo.charging.settle.settle_request_empty");
	}

	const std::string patientId = dto->patientId.getValue("");
	const std::string encounterId = dto->encounterId.getValue("");
	const std::string accountId = dto->accountId.getValue("");
	if (isBlank(patientId) || patientId.size() > 64) {
		return ZH_WORDS_GETTER("ovo.charging.settle.patient_id_invalid");
	}
	if (isBlank(encounterId) || encounterId.size() > 64) {
		return ZH_WORDS_GETTER("ovo.charging.settle.encounter_id_invalid");
	}
	if (isBlank(accountId) || accountId.size() > 64) {
		return ZH_WORDS_GETTER("ovo.charging.settle.account_id_invalid");
	}

	if (!dto->chargeItemIds || dto->chargeItemIds->empty()) {
		return ZH_WORDS_GETTER("ovo.charging.settle.charge_items_empty");
	}
	if (dto->chargeItemIds->size() > MAX_SETTLE_ITEM_COUNT) {
		return ZH_WORDS_GETTER("ovo.charging.settle.charge_items_too_many");
	}

	std::set<std::string> uniqueChargeItemIds;
	for (const auto &chargeItemId : *dto->chargeItemIds.get()) {
		const std::string value = chargeItemId.getValue("");
		if (isBlank(value) || value.size() > 64) {
			return ZH_WORDS_GETTER("ovo.charging.settle.charge_item_id_invalid");
		}
		if (!uniqueChargeItemIds.insert(value).second) {
			return ZH_WORDS_GETTER("ovo.charging.settle.charge_item_id_duplicate");
		}
	}

	const double displayAmount = dto->displayAmount.getValue(0);
	if (!std::isfinite(displayAmount) || displayAmount <= 0 ||
		displayAmount > MAX_MONEY_AMOUNT) {
		return ZH_WORDS_GETTER("ovo.charging.settle.display_amount_invalid");
	}

	if (!dto->paymentDetails || dto->paymentDetails->empty()) {
		return ZH_WORDS_GETTER("ovo.charging.settle.payment_details_empty");
	}
	if (dto->paymentDetails->size() > 10) {
		return ZH_WORDS_GETTER("ovo.charging.settle.payment_details_too_many");
	}

	const std::set<std::string> allowedPaymentTypes = {
		"CASH", "WECHAT", "ALIPAY", "BANK_CARD", "ACCOUNT", "INSURANCE"};
	double paymentAmount = 0;
	for (const auto &paymentDetail : *dto->paymentDetails.get()) {
		if (!paymentDetail) {
			return ZH_WORDS_GETTER("ovo.charging.settle.payment_detail_empty");
		}

		const std::string payEnum = paymentDetail->payEnum.getValue("");
		if (!isAllowedValue(payEnum, allowedPaymentTypes)) {
			return ZH_WORDS_GETTER("ovo.charging.settle.pay_enum_invalid");
		}

		const double amount = paymentDetail->amount.getValue(0);
		if (!std::isfinite(amount) || amount <= 0 ||
			amount > MAX_MONEY_AMOUNT) {
			return ZH_WORDS_GETTER("ovo.charging.settle.payment_amount_invalid");
		}
		paymentAmount += amount;
		if (!std::isfinite(paymentAmount) ||
			paymentAmount > MAX_MONEY_AMOUNT) {
			return ZH_WORDS_GETTER("ovo.charging.settle.payment_total_too_large");
		}

		const std::string payTransNo = paymentDetail->payTransNo.getValue("");
		if (payTransNo.size() > 255) {
			return ZH_WORDS_GETTER("ovo.charging.settle.pay_trans_no_too_long");
		}
		if ((payEnum == "WECHAT" || payEnum == "ALIPAY" ||
			 payEnum == "BANK_CARD") &&
			isBlank(payTransNo)) {
			return ZH_WORDS_GETTER("ovo.charging.settle.pay_trans_no_required");
		}
	}

	if (std::fabs(paymentAmount - displayAmount) > MONEY_EPSILON) {
		return ZH_WORDS_GETTER("ovo.charging.settle.payment_total_invalid");
	}

	const std::string contractNo = dto->contractNo.getValue("");
	if (contractNo.size() > 255) {
		return ZH_WORDS_GETTER("ovo.charging.settle.contract_no_too_long");
	}

	const std::string billDate = dto->billDate.getValue("");
	if (!isValidDateTime(billDate)) {
		return ZH_WORDS_GETTER("ovo.charging.settle.bill_date_invalid");
	}

	const std::string remark = dto->remark.getValue("");
	if (remark.size() > 255) {
		return ZH_WORDS_GETTER("ovo.charging.settle.remark_too_long");
	}
	return "";
}
