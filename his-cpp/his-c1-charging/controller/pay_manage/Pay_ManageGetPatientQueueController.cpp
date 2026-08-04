#include "stdafx.h"
#include "Pay_ManageGetPatientQueueController.h"
#include "service/pay_manage/Pay_ManageGetPatientQueueService.h"

Pay_ManageGetPatientQueuePageJsonVO::Wrapper Pay_ManageGetPatientQueueController::execQueryPay_ManageGetPatientQueue(const Pay_ManageGetPatientQueueQuery::Wrapper& query)
{
    auto vo = Pay_ManageGetPatientQueuePageJsonVO::createShared();
    Pay_ManageGetPatientQueueService service;
    auto pageDTO = service.listAll(query);
    vo->success(pageDTO);
    return vo;
}
