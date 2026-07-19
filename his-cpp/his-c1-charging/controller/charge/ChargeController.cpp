#include"ChargeController.h"
GetPatientQueueJsonVO::Wrapper ChargeController::execQueryPatientQueue(const GetPatientQueueQuery::Wrapper& query)
{
	auto vo = GetPatientQueueJsonVO::createShared();
	return vo;
}