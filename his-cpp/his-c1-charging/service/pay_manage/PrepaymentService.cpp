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

    // Validate total amount
    if (totalAmount <= 0)
    {
        throw std::runtime_error("total amount must be greater than 0");
    }

    UuidFacade uf;
    std::string paymentId = uf.genUuid();
    std::string transactionNo = "TXN" + paymentId.substr(0, 8);
    std::string now = SimpleDateTimeFormat::format();

    // Get latest account snapshot for balance chain
    PrepaymentDAO dao;
    auto snap = dao.getAccountSnapshot(inpatientNo);
    double afterBalance = snap.balance + totalAmount;

    // Insert via native SQL
    dao.insertPrepayment(
        paymentId, now, "01xq", operatorName,
        snap.accountId, inpatientNo,
        "CHARGE", totalAmount, totalAmount, 0.0,
        transactionNo, payText, now,
        snap.balance, afterBalance,
        snap.predecessorId);

    // Build response
    response->paymentId = paymentId;
    response->transactionNo = transactionNo;
    response->encounterId = inpatientNo;   // TODO: resolve encounterId from inpatientNo
    response->inpatientNo = inpatientNo;
    response->amount = totalAmount;
    response->statusEnum = "SUCCESS";
    response->createTime = now;

    return response;
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

    // Validate total amount
    if (totalAmount <= 0)
    {
        throw std::runtime_error("total refund amount must be greater than 0");
    }

    UuidFacade uf;
    std::string paymentId = uf.genUuid();
    std::string transactionNo = "TXN" + paymentId.substr(0, 8);
    std::string now = SimpleDateTimeFormat::format();

    // Get latest account snapshot for balance chain
    PrepaymentDAO dao;
    auto snap = dao.getAccountSnapshot(inpatientNo);

    // Check sufficient balance
    if (totalAmount > snap.balance)
    {
        throw std::runtime_error("insufficient balance: available="
            + std::to_string(snap.balance) + ", refund=" + std::to_string(totalAmount));
    }
    double afterBalance = snap.balance - totalAmount;

    // Insert via native SQL
    dao.insertPrepayment(
        paymentId, now, "01xq", operatorName,
        snap.accountId, inpatientNo,
        "REFUND", totalAmount, 0.0, totalAmount,
        transactionNo, payText, now,
        snap.balance, afterBalance,
        snap.predecessorId);

    // Build response
    response->paymentId = paymentId;
    response->transactionNo = transactionNo;
    response->encounterId = inpatientNo;   // TODO: resolve encounterId from inpatientNo
    response->inpatientNo = inpatientNo;
    response->amount = totalAmount;
    response->statusEnum = "SUCCESS";
    response->createTime = now;

    return response;
}

PrepaymentPageDTO::Wrapper PrepaymentService::queryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    auto page = PrepaymentPageDTO::createShared();
    PrepaymentDAO dao;

    // 统计并查询
    uint64_t total = dao.count(query);
    auto doList = dao.selectWithPage(query);

    // DO映射到DTO
    auto rows = oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
    for (auto& record : doList)
    {
        auto dto = PrepaymentRecordDTO::createShared();
        dto->prepaymentId = record.getId();
        dto->inpatientNo = record.getAccountCode();
        dto->patientName = record.getAccountCode();  // TODO: 关联患者表获取真实姓名
        dto->amount = record.getAmount();
        dto->prepayType = record.getPayEnum();
        dto->method = record.getPayTransText();
        dto->operatorName = record.getCreatedBy();
        dto->createTime = record.getPayTransDate();
        rows->push_back(dto);
    }

    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(total);
    page->calcPages();
    page->rows = rows;

    return page;
}
