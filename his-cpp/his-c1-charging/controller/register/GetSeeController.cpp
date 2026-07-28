#include "stdafx.h"
#include "GetSeeController.h"
#include"../../service/register/GetSeeService.h"

GetSeePageJsonVO::Wrapper GetSeeController::execQueryGetSee(const GetSeeQuery::Wrapper& query)
{
	GetSeeService service;
	auto result = service.selectSee(query);
	auto jvo = GetSeePageJsonVO::createShared();
	jvo->success(result);
	return jvo;
}