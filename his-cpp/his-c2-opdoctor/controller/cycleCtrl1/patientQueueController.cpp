#include "stdafx.h"
#include "patientQueueController.h"


PatientQueueJsonVO::Wrapper PatientQueueController::execQueryQueue(const PatientQueueQuery::Wrapper& query)
{
	PatientQueueService service;
	auto vo = PatientQueueJsonVO::createShared();
	vo->success(service.getPatientQueueList(query, "", ""));
	return vo;
}

