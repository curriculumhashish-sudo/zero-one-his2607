#include "stdafx.h"
#include "Pay_ManageGetPatientDetailsController.h"
#include "service/pay_manage/Pay_ManageGetPatientDetailsService.h"

Pay_ManageGetPatientDetailsVO::Wrapper Pay_ManageGetPatientDetailsController::execPay_ManageGetPatientDetails(const Pay_ManageGetPatientDetailsQuery::Wrapper& query)
{
    auto vo = Pay_ManageGetPatientDetailsVO::createShared();
    Pay_ManageGetPatientDetailsService service;
    auto dto = service.getDetails(query);
    vo->success(dto);
    return vo;
}
