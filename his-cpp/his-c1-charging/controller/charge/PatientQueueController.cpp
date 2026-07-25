#include "PatientQueueController.h"
#include "service/charge/GetPatientQueueService.h"

// 获取患者队列（分页+条件查询）
GetPatientQueueJsonVO::Wrapper PatientQueueController::execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query)
{
    auto vo = GetPatientQueueJsonVO::createShared();
    GetPatientQueueService service;
    auto pageDTO = service.listAll(query);
    vo->success(pageDTO);
    return vo;
}
