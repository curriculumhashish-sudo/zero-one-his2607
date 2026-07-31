#include "stdafx.h"
#include "ChargeConfirmService.h"
#include "SimpleDateTimeFormat.h"
#include "../../dao/register/ChargeConfirmDAO.h"
#include <chrono>
#include <random>

static std::string generateSimpleId() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return std::to_string(ms) + std::to_string(dis(gen));
}

bool ChargeConfirmService::confirmPayment(const ChargeConfirmDTO::Wrapper& dto, const PayloadDTO& payload)
{
    // Validate parameters
    if (!dto->visitNumber || dto->visitNumber->empty())
    {
        return false;
    }
    if (!dto->amount || dto->amount <= 0)
    {
        return false;
    }
    if (!dto->paymentMethod || dto->paymentMethod < 1 || dto->paymentMethod > 4)
    {
        return false;
    }

    // Get current time as transaction time
    std::string transactionTime = SimpleDateTimeFormat::format();

    // Get operator info from token
    std::string operatorId = payload.getId();
    std::string operatorName = payload.getUsername();

    // Load patient info
    std::string visitNumber = dto->visitNumber.getValue({});
    std::string patientId = dto->patientId.getValue({});
    std::string patientName = dto->name.getValue({});
    std::string phone = dto->phone.getValue({});
    oatpp::Float64 amount = dto->amount;
    std::string paymentDraft = dto->paymentDraft.getValue({});
    std::string registrationType = dto->registrationType.getValue({});
    oatpp::Int32 paymentMethod = dto->paymentMethod;

    // Generate business keys
    std::string reconciliationId = generateSimpleId();
    std::string invoiceId = generateSimpleId();
    std::string invoiceNo = "INV" + generateSimpleId();
    std::string detailId = generateSimpleId();

    // Empty strings for missing params
    std::string emptyAccountId = "";
    std::string emptyChargeItemIds = "";

    // Ensure busNo is not empty
    std::string busNo = visitNumber.empty() ? ("BUS" + generateSimpleId()) : visitNumber;

    // Convert payment method int to string
    std::string paymentMethodStr = "CASH";
    if (paymentMethod == 2) paymentMethodStr = "WECHAT";
    else if (paymentMethod == 3) paymentMethodStr = "ALIPAY";
    else if (paymentMethod == 4) paymentMethodStr = "BANK";

    // Execute business logic
    ChargeConfirmDAO dao;
    int version = 1;

    // Update payment reconciliation status
    int updateRes = dao.updatePaymentReconciliation(
        paymentDraft,
        paymentMethodStr,
        reconciliationId,
        transactionTime,
        version
    );

    if (updateRes <= 0) {
        return false;
    }

    // Insert payment detail
    dao.insertPaymentDetail(detailId, reconciliationId, emptyAccountId, paymentMethodStr, amount, amount, transactionTime, "SUCCESS", "PAYMENT");

    // Insert payment static info
    dao.insertPaymentStatic(reconciliationId, paymentMethodStr, paymentMethodStr, ZH_WORDS_GETTER("macchiato.charging.register.summary1"), amount, visitNumber);

    // Update charge item status to paid
    int itemsUpdated = dao.updateChargeItemToPaid(paymentDraft);

    if (itemsUpdated <= 0) {
        return false;
    }

    // Insert invoice record
    dao.insertInvoice(invoiceId, patientId, reconciliationId, detailId, busNo, invoiceNo, paymentMethodStr, amount, emptyChargeItemIds, transactionTime, paymentMethodStr);

    // Update encounter status to registered
    dao.updateEncounterToRegistered(visitNumber, version);

    return true;
}
