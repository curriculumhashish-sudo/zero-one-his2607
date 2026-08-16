#include "stdafx.h"
#include "PrepaymentService.h"
#include "../../dao/pay_manage/PrepaymentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

PrepaymentAddResponseDTO::Wrapper PrepaymentService::addPrepayment(const PrepaymentAddDTO::Wrapper& dto)
{
    auto response = PrepaymentAddResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    std::string operatorId = dto->operatorId.getValue("");
    std::string operatorName = dto->operatorName.getValue("");
    std::string paymentDraftId = dto->paymentDraftId.getValue("");
    std::string remark = dto->remark.getValue("");

    // Calculate total amount from payment details
    double totalAmount = 0.0;
    std::ostringstream payTextBuilder;
    bool first = true;
    if (dto->paymentDetails)
    {
        for (auto& detail : *dto->paymentDetails)
        {
            double detailAmount = detail->amount.getValue(0.0);
            std::string channel = detail->payChannelCode.getValue("CASH");
            totalAmount += detailAmount;
            if (!first) payTextBuilder << "|";
            else first = false;
            payTextBuilder << channel << ":" << detailAmount;
        }
    }
    if (!remark.empty()) payTextBuilder << "|" << remark;
    std::string payText = payTextBuilder.str();

    if (totalAmount <= 0)
    {
        throw std::runtime_error("total amount must be greater than 0");
    }

    UuidFacade uf;
    std::string now = SimpleDateTimeFormat::format();
    std::string tenantId = "01xq";

    // Single DAO instance for transaction scope
    PrepaymentDAO dao;

    // Step 1: Resolve real encounter info from inpatient number
    auto enc = dao.resolveEncounter(inpatientNo);

    // Step 2: Get latest detail balance for chain linking
    auto snap = dao.getAccountSnapshot(enc.accountCode);
    double afterBalance = snap.balance + totalAmount;

    // Generate IDs
    std::string paymentId = uf.genUuid();
    std::string reconciliationId = uf.genUuid();   // PK of fin_payment_reconciliation
    std::string transactionNo = "TXN" + paymentId.substr(0, 8);

    // Step 3: Insert payment reconciliation main record
    dao.insertReconciliation(
        reconciliationId, now, tenantId, operatorId,
        paymentId, "PREPAYMENT_CHARGE",
        enc.patientId, enc.encounterId, enc.accountId,
        totalAmount, 0.0, now);

    // Step 4: Insert prepayment detail record
    dao.insertPrepayment(
        paymentId, now, tenantId, operatorName,
        reconciliationId,
        enc.accountId, enc.accountCode,
        "CHARGE", totalAmount, totalAmount, 0.0,
        transactionNo, payText, now,
        snap.balance, afterBalance,
        snap.predecessorId);

    // Step 5: Update account balance
    double newBalance = enc.balance + totalAmount;
    dao.updateAccountBalance(enc.accountId, newBalance, now);

    // Build response with real encounter data
    response->paymentId = paymentId;
    response->transactionNo = transactionNo;
    response->encounterId = enc.encounterId;
    response->inpatientNo = inpatientNo;
    response->amount = totalAmount;
    response->balance = newBalance;
    response->statusEnum = "SUCCESS";
    response->createTime = now;

    return response;
}

PrepaymentPageDTO::Wrapper PrepaymentService::queryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    PrepaymentDAO dao;

    auto page = PrepaymentPageDTO::createShared();
    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);

    uint64_t total = dao.count(query);
    page->total = total;

    if (total > 0)
    {
        auto records = dao.selectWithPage(query);
        oatpp::List<PrepaymentRecordDTO::Wrapper> recordList =
            oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
        for (auto& doItem : records)
        {
            auto dto = PrepaymentRecordDTO::createShared();
            dto->prepaymentId = doItem.getId();
            dto->prepayType = doItem.getPayEnum();
            dto->amount = doItem.getAmount();
            dto->createTime = doItem.getPayTransDate();
            dto->operatorName = doItem.getCreatedBy();
            recordList->push_back(dto);
        }
        page->rows = recordList;
    }

    return page;
}

PrepaymentRefundResponseDTO::Wrapper PrepaymentService::refundPrepayment(const PrepaymentRefundDTO::Wrapper& dto)
{
    auto response = PrepaymentRefundResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    std::string operatorId = dto->operatorId.getValue("");
    std::string operatorName = dto->operatorName.getValue("");
    std::string paymentDraftId = dto->paymentDraftId.getValue("");
    std::string reason = dto->reason.getValue("");
    std::string remark = dto->remark.getValue("");

    // Calculate total refund amount from payment details
    double totalAmount = 0.0;
    std::ostringstream payTextBuilder;
    bool first = true;
    if (dto->paymentDetails)
    {
        for (auto& detail : *dto->paymentDetails)
        {
            double detailAmount = detail->amount.getValue(0.0);
            std::string channel = detail->payChannelCode.getValue("CASH");
            totalAmount += detailAmount;
            if (!first) payTextBuilder << "|";
            else first = false;
            payTextBuilder << channel << ":" << detailAmount;
        }
    }
    if (!reason.empty()) payTextBuilder << "|reason:" << reason;
    if (!remark.empty()) payTextBuilder << "|" << remark;
    std::string payText = payTextBuilder.str();

    if (totalAmount <= 0)
    {
        throw std::runtime_error("total refund amount must be greater than 0");
    }

    UuidFacade uf;
    std::string now = SimpleDateTimeFormat::format();
    std::string tenantId = "01xq";

    // Single DAO instance for transaction scope
    PrepaymentDAO dao;

    // Step 1: Resolve real encounter info from inpatient number
    auto enc = dao.resolveEncounter(inpatientNo);

    // Step 2: Get latest detail balance for chain linking
    auto snap = dao.getAccountSnapshot(enc.accountCode);

    // Check sufficient balance (use adm_account balance as source of truth)
    if (totalAmount > enc.balance)
    {
        throw std::runtime_error("insufficient balance: available="
            + std::to_string(enc.balance) + ", refund=" + std::to_string(totalAmount));
    }
    double afterBalance = snap.balance - totalAmount;

    // Generate IDs
    std::string paymentId = uf.genUuid();
    std::string reconciliationId = uf.genUuid();
    std::string transactionNo = "TXN" + paymentId.substr(0, 8);

    // Step 3: Insert payment reconciliation main record
    dao.insertReconciliation(
        reconciliationId, now, tenantId, operatorId,
        paymentId, "PREPAYMENT_REFUND",
        enc.patientId, enc.encounterId, enc.accountId,
        0.0, totalAmount, now);

    // Step 4: Insert prepayment detail record (refund)
    dao.insertPrepayment(
        paymentId, now, tenantId, operatorName,
        reconciliationId,
        enc.accountId, enc.accountCode,
        "REFUND", totalAmount, 0.0, totalAmount,
        transactionNo, payText, now,
        snap.balance, afterBalance,
        snap.predecessorId);

    // Step 5: Update account balance
    double newBalance = enc.balance - totalAmount;
    dao.updateAccountBalance(enc.accountId, newBalance, now);

    // Build response with real encounter data
    response->paymentId = paymentId;
    response->transactionNo = transactionNo;
    response->encounterId = enc.encounterId;
    response->inpatientNo = inpatientNo;
    response->amount = totalAmount;
    response->balance = newBalance;
    response->statusEnum = "SUCCESS";
    response->createTime = now;

    return response;
}
