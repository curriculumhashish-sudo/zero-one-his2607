#include "PatientQueueController.h"

// 获取患者队列
GetPatientQueueJsonVO::Wrapper PatientQueueController::execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query)
{
    auto vo = GetPatientQueueJsonVO::createShared();
    return vo;
}
