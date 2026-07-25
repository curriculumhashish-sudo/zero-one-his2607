#include "stdafx.h"
#include "patientQueueController.h"
#include "service/cycleCtrl1/patientQueueService.h"


PatientQueueJsonVO::Wrapper PatientQueueController::execQueryQueue(const PatientQueueQuery::Wrapper& query)
{
	PatientQueueService service;
	auto vo = PatientQueueJsonVO::createShared();
	vo->success(service.getPatientQueueList(query));
	return vo;
}

