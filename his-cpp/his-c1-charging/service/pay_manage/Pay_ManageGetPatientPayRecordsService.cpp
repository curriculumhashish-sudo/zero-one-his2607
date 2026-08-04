#include "stdafx.h"
#include "Pay_ManageGetPatientPayRecordsService.h"
#include "dao/pay_manage/Pay_ManageGetPatientPayRecordsDAO.h"

// Prevent ODBC/MySQL type conflict
#include "NacosClient.h"

// ============================================================================
// Helper methods: enum translation
// ============================================================================

std::string Pay_ManageGetPatientPayRecordsService::translatePayType(const std::string& code)
{
    if (code == "CASH")      return ZH_WORDS_GETTER("byj.charging.pay_manage.pay_type_cash");
    if (code == "WECHAT")    return ZH_WORDS_GETTER("byj.charging.pay_manage.pay_type_wechat");
    if (code == "ALIPAY")    return ZH_WORDS_GETTER("byj.charging.pay_manage.pay_type_alipay");
    if (code == "BANK_CARD") return ZH_WORDS_GETTER("byj.charging.pay_manage.pay_type_bank_card");
    if (code == "INSURANCE") return ZH_WORDS_GETTER("byj.charging.pay_manage.pay_type_insurance");
    return code;
}

std::string Pay_ManageGetPatientPayRecordsService::translateServiceType(const std::string& code)
{
    if (code == "PREPAY")     return ZH_WORDS_GETTER("byj.charging.pay_manage.service_type_prepay");
    if (code == "SETTLEMENT") return ZH_WORDS_GETTER("byj.charging.pay_manage.service_type_settlement");
    if (code == "REFUND")     return ZH_WORDS_GETTER("byj.charging.pay_manage.service_type_refund");
    return code;
}

std::string Pay_ManageGetPatientPayRecordsService::translateStatus(const std::string& code)
{
    if (code == "COMPLETED")  return ZH_WORDS_GETTER("byj.charging.pay_manage.status_completed");
    if (code == "PENDING")    return ZH_WORDS_GETTER("byj.charging.pay_manage.status_pending");
    if (code == "CANCELLED")  return ZH_WORDS_GETTER("byj.charging.pay_manage.status_cancelled");
    return code;
}

// ============================================================================
// Main method: query, convert, assemble
// ============================================================================

Pay_ManageGetPatientPayRecordsPageDTO::Wrapper Pay_ManageGetPatientPayRecordsService::listAll(
    const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query)
{
    // Step 1: Query total count
    Pay_ManageGetPatientPayRecordsDAO dao;
    uint64_t totalCount = dao.count(query);

    // Step 2: Paginated query for current page data
    auto doList = dao.selectWithPage(query);

    // Step 3: Initialize page DTO and fill pagination metadata
    auto pageDTO = Pay_ManageGetPatientPayRecordsPageDTO::createShared();
    pageDTO->pageIndex = query->pageIndex.getValue(1);
    pageDTO->pageSize  = query->pageSize.getValue(10);
    pageDTO->total = static_cast<int64_t>(totalCount);
    pageDTO->calcPages();
    pageDTO->rows = oatpp::List<Pay_ManageGetPatientPayRecordsDTO::Wrapper>::createShared();

    if (doList.empty())
        return pageDTO;

    // Step 4: Convert each DO to DTO and translate fields
    for (auto& dobj : doList)
    {
        auto dto = Pay_ManageGetPatientPayRecordsDTO::createShared();

        // Direct mapping fields
        dto->pay_time = dobj.getPayTime().empty() ? oatpp::String() : oatpp::String(dobj.getPayTime());
        dto->operateman = dobj.getOperateman().empty() ? oatpp::String() : oatpp::String(dobj.getOperateman());
        dto->bill_number = dobj.getBillNumber().empty() ? oatpp::String() : oatpp::String(dobj.getBillNumber());

        // Translated fields
        dto->service_type = translateServiceType(dobj.getServiceType());
        dto->pay_type = translatePayType(dobj.getPayType());
        dto->status = translateStatus(dobj.getStatus());

        // Type conversion: string -> Float64 for amount
        try { dto->cost = std::stod(dobj.getCost()); }
        catch (...) { dto->cost = 0.0; }

        // Add to current page data list
        pageDTO->addData(dto);
    }

    return pageDTO;
}
