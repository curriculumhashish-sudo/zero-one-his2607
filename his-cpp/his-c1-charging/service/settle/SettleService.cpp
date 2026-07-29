#include "stdafx.h"
#include "SettleService.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "TransactionManager.h"
#include "dao/settle/SettleDAO.h"
#include "id/SnowFlake.h"
#include "SimpleDateTimeFormat.h"
#include "service/settle/SettlementValidator.h"

namespace {
	constexpr int64_t MONEY_SCALE = 1000000;

	std::string nextId() {
		const uint64_t generatedId = SnowFlake::getInstance()->nextId();
		if (generatedId == 0) {
			throw std::runtime_error(ZH_WORDS_GETTER("ovo.charging.settle.generate_id_failed"));
		}
		return std::to_string(generatedId);
	}

	int64_t decimalToMoneyUnit(const std::string &value) {
		if (value.empty()) {
			return 0;
		}
		const double amount = std::stod(value);
		const double maxAmount =
			static_cast<double>(std::numeric_limits<int64_t>::max()) /
			MONEY_SCALE;
		if (!std::isfinite(amount) || amount < 0 || amount > maxAmount) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.charge_amount_out_of_range"));
		}
		return std::llround(amount * MONEY_SCALE);
	}

	int64_t amountToMoneyUnit(double value) {
		return std::llround(value * MONEY_SCALE);
	}

	std::string moneyUnitToString(int64_t value) {
		std::ostringstream stream;
		stream << value / MONEY_SCALE << "."
			   << std::setfill('0') << std::setw(6)
			   << value % MONEY_SCALE;
		return stream.str();
	}

	std::string joinChargeItemIds(
		const std::vector<std::string> &chargeItemIds) {
		std::ostringstream stream;
		for (size_t index = 0; index < chargeItemIds.size(); ++index) {
			if (index > 0) {
				stream << ",";
			}
			stream << chargeItemIds[index];
		}
		return stream.str();
	}

	std::string resolveOperatorId(const PayloadDTO &payload) {
		if (!payload.getId().empty()) {
			return payload.getId();
		}
		if (!payload.getUsername().empty()) {
			return payload.getUsername();
		}
		throw std::invalid_argument(ZH_WORDS_GETTER("ovo.charging.settle.operator_not_found"));
	}

	std::string toUpperAscii(std::string value) {
		std::transform(value.begin(), value.end(), value.begin(),
			[](unsigned char character) {
				return static_cast<char>(std::toupper(character));
			});
		return value;
	}
}

SettleResultDTO::Wrapper SettleService::settle(
	const SettleRequestDTO::Wrapper &dto,
	const PayloadDTO &payload) {
	const std::string validationMessage =
		SettlementValidator::validateSettleRequest(dto);
	if (!validationMessage.empty()) {
		throw std::invalid_argument(validationMessage);
	}

	const std::string patientId = dto->patientId.getValue("");
	const std::string encounterId = dto->encounterId.getValue("");
	const std::string accountId = dto->accountId.getValue("");
	const std::string operatorId = resolveOperatorId(payload);

	std::vector<std::string> chargeItemIds;
	chargeItemIds.reserve(dto->chargeItemIds->size());
	for (const auto &chargeItemId : *dto->chargeItemIds.get()) {
		chargeItemIds.push_back(chargeItemId.getValue(""));
	}

	SettleDAO dao;
	TransactionManager transaction(&dao);
	try {
		const PtrSettlementAccountDO account =
			dao.selectAccountForUpdate(patientId, encounterId, accountId);
		if (!account) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.account_mismatch"));
		}

		const std::string accountStatus =
			toUpperAscii(account->getStatusEnum());
		if (accountStatus == "INACTIVE" || accountStatus == "DISABLED") {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.account_unavailable"));
		}

		const std::string billingStatus =
			toUpperAscii(account->getBillingStatusEnum());
		if (billingStatus == "SETTLED" || billingStatus == "CLOSED") {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.account_already_settled"));
		}

		const std::string requestContractNo = dto->contractNo.getValue("");
		const std::string accountContractNo = account->getContractNo();
		if (!requestContractNo.empty() && !accountContractNo.empty() &&
			requestContractNo != accountContractNo) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.contract_mismatch"));
		}
		const std::string effectiveContractNo =
			requestContractNo.empty() ? accountContractNo : requestContractNo;
		if (!effectiveContractNo.empty() &&
			dao.isInsuranceContract(account->getTenantId(), effectiveContractNo)) {
			throw std::logic_error(
				ZH_WORDS_GETTER("ovo.charging.settle.insurance_contract_requires_api"));
		}

		const std::list<PtrSettlementChargeItemDO> chargeItems =
			dao.selectChargeItemsForUpdate(
				patientId, encounterId, accountId, chargeItemIds);
		if (chargeItems.size() != chargeItemIds.size()) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.charge_item_mismatch"));
		}

		int64_t chargeAmount = 0;
		for (const auto &chargeItem : chargeItems) {
			const std::string statusEnum = chargeItem->getStatusEnum();
			if (statusEnum != "DRAFT" && statusEnum != "UNSETTLED") {
				throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.charge_item_status_invalid"));
			}
			const int64_t itemAmount =
				decimalToMoneyUnit(chargeItem->getTotalPrice());
			if (itemAmount < 0 ||
				chargeAmount > std::numeric_limits<int64_t>::max() - itemAmount) {
				throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.charge_amount_out_of_range"));
			}
			chargeAmount += itemAmount;
		}

		const int64_t displayAmount =
			amountToMoneyUnit(dto->displayAmount.getValue(0));
		if (chargeAmount != displayAmount) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.charge_amount_mismatch"));
		}

		int64_t tenderedAmount = 0;
		int64_t accountPaymentAmount = 0;
		for (const auto &paymentDetail : *dto->paymentDetails.get()) {
			const std::string payEnum =
				paymentDetail->payEnum.getValue("");
			const int64_t detailAmount =
				amountToMoneyUnit(paymentDetail->amount.getValue(0));
			tenderedAmount += detailAmount;
			if (payEnum == "ACCOUNT") {
				accountPaymentAmount += detailAmount;
			}
			if (payEnum == "INSURANCE") {
				throw std::logic_error(
					ZH_WORDS_GETTER("ovo.charging.settle.insurance_payment_requires_api"));
			}
		}
		if (tenderedAmount != displayAmount) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.settle_payment_total_invalid"));
		}
		const int64_t accountBalance =
			decimalToMoneyUnit(account->getBalance());
		if (accountPaymentAmount > accountBalance) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.account_balance_insufficient"));
		}
		const int64_t accountBalanceAfter =
			accountBalance - accountPaymentAmount;

		const std::string settledAt = SimpleDateTimeFormat::format();
		const std::string tenantId = account->getTenantId();
		const std::string chargeItemIdText =
			joinChargeItemIds(chargeItemIds);
		const std::string paymentId = nextId();
		const std::string invoiceId = nextId();
		const std::string paymentNo = "IPAY" + paymentId;
		const std::string invoiceNo = "IV" + invoiceId;
		const std::string amountText = moneyUnitToString(displayAmount);

		SettlementPaymentDO payment;
		ZO_STAR_DOMAIN_DTO_TO_DO(
			payment, dto,
			PatientId, patientId,
			EncounterId, encounterId,
			AccountId, accountId,
			BillDate, billDate);
		payment.setId(paymentId);
		payment.setTenantId(tenantId);
		payment.setCreatedAt(settledAt);
		payment.setUpdatedAt(settledAt);
		payment.setCreatedBy(operatorId);
		payment.setUpdatedBy(operatorId);
		payment.setIsDeleted(0);
		payment.setVersion(0);
		payment.setStatusEnum("CONFIRMED");
		payment.setRelationId(accountId);
		payment.setPaymentNo(paymentNo);
		payment.setPaymentEnum(
			dto->paymentDetails->size() == 1
				? dto->paymentDetails->at(0)->payEnum.getValue("")
				: "MIXED");
		payment.setKindEnum("SETTLEMENT");
		payment.setEntererId(operatorId);
		payment.setIssuerEnum("HOSPITAL");
		payment.setOutcomeEnum("SUCCESS");
		payment.setTenderedAmount(amountText);
		payment.setReturnedAmount("0.000000");
		payment.setDisplayAmount(amountText);
		payment.setPrintCount(0);
		payment.setContractNo(effectiveContractNo);
		payment.setChargeItemIds(chargeItemIdText);
		payment.setInvoiceId(invoiceId);
		payment.setYbClearFlag(0);

		std::list<SettlementPaymentDetailDO> paymentDetailData;
		for (const auto &paymentDto : *dto->paymentDetails.get()) {
			SettlementPaymentDetailDO &detail =
				paymentDetailData.emplace_back();
			ZO_STAR_DOMAIN_DTO_TO_DO(
				detail, paymentDto,
				PayEnum, payEnum,
				PayTransNo, payTransNo);
			detail.setId(nextId());
			detail.setTenantId(tenantId);
			detail.setCreatedAt(settledAt);
			detail.setUpdatedAt(settledAt);
			detail.setCreatedBy(operatorId);
			detail.setUpdatedBy(operatorId);
			detail.setIsDeleted(0);
			detail.setVersion(0);
			detail.setReconciliationId(paymentId);
			detail.setTargetEnum("PATIENT");
			detail.setAccountId(accountId);
			detail.setAccountCode(account->getBusNo());
			detail.setAmount(moneyUnitToString(
				amountToMoneyUnit(paymentDto->amount.getValue(0))));
			detail.setReturnAmount("0.000000");
			detail.setChargeAmount(detail.getAmount());
			detail.setPayTransText(dto->remark.getValue(""));
			detail.setPayTransDate(dto->billDate.getValue(""));
			detail.setBeforeBalance(moneyUnitToString(accountBalance));
			detail.setAfterBalance(moneyUnitToString(accountBalanceAfter));
			detail.setResultEnum("SUCCESS");
		}

		SettlementInvoiceDO invoice;
		invoice.setId(invoiceId);
		invoice.setTenantId(tenantId);
		invoice.setCreatedAt(settledAt);
		invoice.setUpdatedAt(settledAt);
		invoice.setCreatedBy(operatorId);
		invoice.setUpdatedBy(operatorId);
		invoice.setIsDeleted(0);
		invoice.setVersion(0);
		invoice.setPatientId(patientId);
		invoice.setReconciliationId(paymentId);
		invoice.setPaymentId(paymentId);
		invoice.setBusNo(invoiceNo);
		invoice.setBillNo(invoiceNo);
		invoice.setTypeCode("INPATIENT");
		invoice.setChargeItemIds(chargeItemIdText);
		invoice.setPaymentTerms(payment.getPaymentEnum());
		invoice.setAmount(amountText);
		invoice.setStatusEnum("DRAFT");

		if (dao.insertPayment(payment) != 1) {
			throw std::runtime_error(ZH_WORDS_GETTER("ovo.charging.settle.insert_payment_failed"));
		}
		if (dao.insertPaymentDetails(paymentDetailData) !=
			static_cast<int>(paymentDetailData.size())) {
			throw std::runtime_error(ZH_WORDS_GETTER("ovo.charging.settle.insert_payment_detail_failed"));
		}
		if (dao.insertInvoice(invoice) != 1) {
			throw std::runtime_error(ZH_WORDS_GETTER("ovo.charging.settle.insert_invoice_failed"));
		}
		if (dao.updateChargeItemsToSettled(
				patientId,
				encounterId,
				accountId,
				chargeItemIds,
				settledAt,
				operatorId) != static_cast<int>(chargeItemIds.size())) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.charge_item_state_changed"));
		}
		const bool closeAccount = dao.countUnsettledChargeItems(
									  patientId, encounterId, accountId) == 0;
		if ((accountPaymentAmount > 0 || closeAccount) &&
			dao.updateAccountAfterSettlement(
				accountId,
				patientId,
				encounterId,
				moneyUnitToString(accountBalanceAfter),
				closeAccount,
				settledAt,
				operatorId,
				account->getVersion()) != 1) {
			throw std::logic_error(ZH_WORDS_GETTER("ovo.charging.settle.account_state_changed"));
		}

		auto result = SettleResultDTO::createShared();
		result->paymentId = paymentId;
		result->paymentNo = paymentNo;
		result->invoiceId = invoiceId;
		result->displayAmount =
			static_cast<double>(displayAmount) / MONEY_SCALE;
		result->statusEnum = "CONFIRMED";
		result->settledAt = settledAt;

		transaction.commit();
		return result;
	}
	catch (...) {
		transaction.rollback();
		throw;
	}
}
