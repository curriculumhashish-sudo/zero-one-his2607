#include "stdafx.h"
#include "SettleController.h"

SettleJsonVO::Wrapper SettleController::execSettle(const SettleRequestDTO::Wrapper& dto)
{
	(void)dto;
	auto vo = SettleJsonVO::createShared();
	vo->success(SettleResultDTO::createShared());
	return vo;
}
