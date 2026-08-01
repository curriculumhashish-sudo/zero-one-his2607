#include "stdafx.h"
#include "PrepaymentService.h"
#include "../../dao/pay_manage/PrepaymentDAO.h"
#include "id/UuidFacade.h"
#include "SimpleDateTimeFormat.h"

PrepaymentAddResponseDTO::Wrapper PrepaymentService::addPrepayment(const PrepaymentAddDTO::Wrapper& dto)
{
    auto response = PrepaymentAddResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    double amount = dto->amount.getValue(0.0);
    response->prepaymentId = "test-uuid-12345";
    response->inpatientNo = inpatientNo;
    response->amount = amount;
    response->paymentMethod = dto->paymentMethod;
    response->createTime = SimpleDateTimeFormat::format();
    return response;
}

PrepaymentRefundResponseDTO::Wrapper PrepaymentService::refundPrepayment(const PrepaymentRefundDTO::Wrapper& dto)
{
    auto response = PrepaymentRefundResponseDTO::createShared();
    std::string inpatientNo = dto->inpatientNo.getValue("");
    double amount = dto->amount.getValue(0.0);
    response->prepaymentId = "test-uuid-67890";
    response->inpatientNo = inpatientNo;
    response->amount = amount;
    response->refundMethod = dto->refundMethod;
    response->createTime = SimpleDateTimeFormat::format();
    return response;
}

PrepaymentPageDTO::Wrapper PrepaymentService::queryPrepayment(const PrepaymentQuery::Wrapper& query)
{
    auto page = PrepaymentPageDTO::createShared();
    page->pageIndex = query->pageIndex.getValue(1);
    page->pageSize = query->pageSize.getValue(10);
    page->total = v_int64(0);
    page->calcPages();
    page->rows = oatpp::List<PrepaymentRecordDTO::Wrapper>::createShared();
    return page;
}
