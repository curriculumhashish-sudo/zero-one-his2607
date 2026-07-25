#include "stdafx.h"
#include "GetCostDetailController.h"

GetCostDetailJsonVO::Wrapper GetCostDetailController::execGetCostDetail(const GetCostDetailQuery::Wrapper& query)
{
	(void)query;
	auto vo = GetCostDetailJsonVO::createShared();
	vo->success(GetCostDetailDTO::createShared());
	return vo;
}
