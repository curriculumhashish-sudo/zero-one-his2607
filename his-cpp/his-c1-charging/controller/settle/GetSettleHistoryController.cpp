#include "stdafx.h"
#include "GetSettleHistoryController.h"

GetSettleHistoryJsonVO::Wrapper GetSettleHistoryController::execGetSettleHistory(const GetSettleHistoryQuery::Wrapper& query)
{
	(void)query;
	auto vo = GetSettleHistoryJsonVO::createShared();
	vo->success(GetSettleHistoryPageDTO::createShared());
	return vo;
}
