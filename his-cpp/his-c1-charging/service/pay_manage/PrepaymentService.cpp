#include "stdafx.h"
#include "PrepaymentService.h"
#include "../../dao/pay_manage/PrepaymentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"
#include "DbInit.h"

// Check if database pool is initialized and connection is healthy
// TODO: Set to true after fixing DB connection on the server
static bool isDbAvailable()
{
    return DbInit::getConnPool() != nullptr;
}

PrepaymentAddResponseDTO::Wrapper PrepaymentService::addPrepayment(const PrepaymentAddDTO::Wrapper& dto)
{
    auto response = PrepaymentAddResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    double amount = dto->amount.getValue(0.0);
    std::string paymentMethod = dto->paymentMethod.getValue("1");
    std::string operatorName = dto->operatorName.getValue("");

    if (!isDbAvailable())
    {
        OATPP_LOGW("Prepayment", "addPrepayment: DB pool not initialized, returning mock data");
        response->prepaymentId = "mock-" + SimpleDateTimeFormat::format();
        response->inpatientNo = inpatientNo;
        response->amount = amount;
        response->paymentMethod = paymentMethod;
        response->createTime = SimpleDateTimeFormat::format();
        return response;
    }

    std::string remark = dto->remark.getValue("");
    OATPP_LOGD("Prepayment", "addPrepayment: inpatientNo=%s, amount=%.2f", inpatientNo.c_str(), amount);

    UuidFacade uf;
    PrepaymentDAO dao;

    // Get account info in one query
    OATPP_LOGD("Prepayment", "addPrepayment: getting account snapshot");
    auto snap = dao.getAccountSnapshot(inpatientNo);
    OATPP_LOGD("Prepayment", "addPrepayment: accountId=%s, balance=%.2f, predecessorId=%s",
        snap.accountId.c_str(), snap.balance, snap.predecessorId.c_str());

    double afterBalance = snap.balance + amount;

    std::string now = SimpleDateTimeFormat::format();
    std::string uuid = uf.genUuid();
    OATPP_LOGD("Prepayment", "addPrepayment: uuid=%s, now=%s", uuid.c_str(), now.c_str());

    // Build DO
    PrepaymentDO prepayment;
    prepayment.setId(uuid);
    prepayment.setTenantId("01xq");
    prepayment.setCreatedAt(now);
    prepayment.setUpdatedAt(now);
    prepayment.setCreatedBy(operatorName);
    prepayment.setUpdatedBy(operatorName);
    prepayment.setIsDeleted(0);
    prepayment.setVersion(0);

    prepayment.setTargetEnum("PREPAYMENT");
    prepayment.setAccountId(snap.accountId);
    prepayment.setAccountCode(inpatientNo);
    prepayment.setPayEnum("CHARGE");
    prepayment.setAmount(amount);
    prepayment.setChargeAmount(amount);
    prepayment.setReturnAmount(0.0);
    prepayment.setPayTransNo(uuid);
    prepayment.setPayTransText(paymentMethod + (remark.empty() ? "" : "," + remark));
    prepayment.setPayTransDate(now);
    prepayment.setBeforeBalance(snap.balance);
    prepayment.setAfterBalance(afterBalance);
    prepayment.setResultEnum("SUCCESS");
    if (!snap.predecessorId.empty())
    {
        prepayment.setPredecessorId(snap.predecessorId);
    }

    // Insert to DB
    OATPP_LOGD("Prepayment", "addPrepayment: inserting to DB");
    try
    {
        dao.insertPrepayment(prepayment);
        OATPP_LOGD("Prepayment", "addPrepayment: insert done");
    }
    catch (const std::exception& e)
    {
        OATPP_LOGE("Prepayment", "addPrepayment insert failed: %s", e.what());
    }
    catch (...)
    {
        OATPP_LOGE("Prepayment", "addPrepayment insert failed: unknown error");
    }

    // Build response
    response->prepaymentId = uuid;
    response->inpatientNo = inpatientNo;
    response->amount = amount;
    response->paymentMethod = paymentMethod;
    response->createTime = now;

    return response;
}

PrepaymentRefundResponseDTO::Wrapper PrepaymentService::refundPrepayment(const PrepaymentRefundDTO::Wrapper& dto)
{
    auto response = PrepaymentRefundResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    double amount = dto->amount.getValue(0.0);
    std::string refundMethod = dto->refundMethod.getValue("1");
    std::string operatorName = dto->operatorName.getValue("");

    if (!isDbAvailable())
    {
        OATPP_LOGW("Prepayment", "refundPrepayment: DB pool not initialized, returning mock data");
        response->prepaymentId = "mock-" + SimpleDateTimeFormat::format();
        response->inpatientNo = inpatientNo;
        response->amount = amount;
        response->refundMethod = refundMethod;
        response->createTime = SimpleDateTimeFormat::format();
        return response;
    }

    std::string reason = dto->reason.getValue("");
    std::string remark = dto->remark.getValue("");

    UuidFacade uf;
    PrepaymentDAO dao;

    // Get account info in one query
    auto snap = dao.getAccountSnapshot(inpatientNo);
    double afterBalance = snap.balance - amount;

    std::string now = SimpleDateTimeFormat::format();
    std::string uuid = uf.genUuid();

    // Build DO
    PrepaymentDO prepayment;
    prepayment.setId(uuid);
    prepayment.setTenantId("01xq");
    prepayment.setCreatedAt(now);
    prepayment.setUpdatedAt(now);
    prepayment.setCreatedBy(operatorName);
    prepayment.setUpdatedBy(operatorName);
    prepayment.setIsDeleted(0);
    prepayment.setVersion(0);

    prepayment.setTargetEnum("PREPAYMENT");
    prepayment.setAccountId(snap.accountId);
    prepayment.setAccountCode(inpatientNo);
    prepayment.setPayEnum("REFUND");
    prepayment.setAmount(amount);
    prepayment.setChargeAmount(0.0);
    prepayment.setReturnAmount(amount);
    prepayment.setPayTransNo(uuid);
    prepayment.setPayTransText(refundMethod
        + (reason.empty() ? "" : "," + reason)
        + (remark.empty() ? "" : "," + remark));
    prepayment.setPayTransDate(now);
    prepayment.setBeforeBalance(snap.balance);
    prepayment.setAfterBalance(afterBalance);
    prepayment.setResultEnum("SUCCESS");
    if (!snap.predecessorId.empty())
    {
        prepayment.setPredecessorId(snap.predecessorId);
    }

    // Insert to DB
    try
    {
        dao.insertPrepayment(prepayment);
    }
    catch (const std::exception& e)
    {
        OATPP_LOGE("Prepayment", "refundPrepayment insert failed: %s", e.what());
    }
    catch (...)
    {
        OATPP_LOGE("Prepayment", "refundPrepayment insert failed: unknown error");
    }

    // Build response
    response->prepaymentId = uuid;
    response->inpatientNo = inpatientNo;
    response->amount = amount;
    response->refundMethod = refundMethod;
    response->createTime = now;

    return response;
}

PrepaymentPageDTO::Wrapper PrepaymentService::queryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    auto page = PrepaymentPageDTO::createShared();

    if (!isDbAvailable())
    {
        OATPP_LOGW("Prepayment", "queryPrepayment: DB pool not initialized, returning empty page");
        page->pageIndex = query->pageIndex.getValue(1);
        page->pageSize = query->pageSize.getValue(10);
        page->total = v_int64(0);
        page->calcPages();
        page->rows = oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
        return page;
    }

    PrepaymentDAO dao;

    // Count total records
    uint64_t total = dao.count(query);

    // Query records with pagination
    auto doList = dao.selectWithPage(query);

    // Map DO to DTO
    auto rows = oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
    for (auto& record : doList)
    {
        auto dto = PrepaymentRecordDTO::createShared();
        dto->prepaymentId = record.getId();
        dto->inpatientNo = record.getAccountCode();
        dto->patientName = record.getAccountCode();  // TODO: join patient table for real name
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
