#include "stdafx.h"
#include "NursingController.h"

NursingRecordListJsonVO::Wrapper NursingController::execListNursingRecords(
	const NursingRecordQuery::Wrapper& query)
{
	auto jvo = NursingRecordListJsonVO::createShared();
	jvo->setStatus(RS_SUCCESS);
	return jvo;
}

NursingRecordSaveJsonVO::Wrapper NursingController::execSaveNursingRecords(
	const NursingRecordSaveRequestDTO::Wrapper& dto)
{
	auto jvo = NursingRecordSaveJsonVO::createShared();
	jvo->setStatus(RS_SUCCESS);
	return jvo;
}
