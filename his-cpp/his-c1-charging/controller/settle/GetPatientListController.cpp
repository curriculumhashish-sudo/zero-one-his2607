#include "stdafx.h"
#include "GetPatientListController.h"
#include "../../service/settle/GetPatientListService.h"

GetPatientListPageJsonVO::Wrapper GetPatientListController::execQueryGetPatientList(const GetPatientListQuery::Wrapper& query)
{
	GetPatientListService service;
	auto result = service.selectPatientList(query);
	auto jvo = GetPatientListPageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}