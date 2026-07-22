#include "Pay_ManageGetPatientDetailsController.h"

// 获取患者缴费项
Pay_ManageGetPatientDetailsVO::Wrapper Pay_ManageGetPatientDetailsController::execPay_ManageGetPatientDetails(const Pay_ManageGetPatientDetailsQuery::Wrapper& query)
{
    auto vo = Pay_ManageGetPatientDetailsVO::createShared();
    return vo;
}
