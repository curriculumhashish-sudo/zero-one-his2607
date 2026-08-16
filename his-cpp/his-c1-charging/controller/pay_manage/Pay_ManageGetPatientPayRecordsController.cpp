#include "stdafx.h"
#include "Pay_ManageGetPatientPayRecordsController.h"
#include "service/pay_manage/Pay_ManageGetPatientPayRecordsService.h"

Pay_ManageGetPatientPayRecordsPageJsonVO::Wrapper Pay_ManageGetPatientPayRecordsController::execQueryPay_ManageGetPatientPayRecords(const Pay_ManageGetPatientPayRecordsQuery::Wrapper& query)
{
    auto vo = Pay_ManageGetPatientPayRecordsPageJsonVO::createShared();
    Pay_ManageGetPatientPayRecordsService service;
    auto pageDTO = service.listAll(query);
    vo->success(pageDTO);
    return vo;
}
