#include "GetPatientDetailsController.h"

// 获取患者缴费项
GetPatientDetailsVO::Wrapper GetPatientDetailsController::execGetPatientDetails(const GetPatientDetailsQuery::Wrapper& query)
{
    auto vo = GetPatientDetailsVO::createShared();
    return vo;
}
